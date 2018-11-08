#pragma once
/*
o("text", ns) to disable the automatic spacing before the text.
o("text", function, n) to add the optional neutral link. its meaning: back, or continue. it is "neutral" because it represents that no action is taken
o("text", function, a) to add an aside link, which doesn't have a keyboard shortcut.
o("text", nullptr) to add a nonclickable link. it still has a keyboard shortcut by default, which can be removed by making it an aside link. (the purpose of this nonclickable keyboard shortcut is to preserve ordering for links afterward)
o(suppress_history) to prevent the text afterward from appearing in the scrollback history.
*/
#include <string>
#include <vector>
#include <functional>
#include <array>
#include <emscripten/emscripten.h>

constexpr int link_count = 8;

//turn \n into <p> tags to improve accessibility and formatting
//this creates unnecessary p tags when the input ends in \n</p> or starts in <p>\n, but that doesn't matter very much
//this creates a tremendous number of unnecessary p tags for <ul> lists, since the browser closes off the tags. that also doesn't matter.
inline std::string convert_newlines_to_paragraph_tags(std::string_view input)
{
	if (input.empty()) return ""; //special case: don't add p tags
	std::string result_string("<p>");
	std::size_t start_of_paragraph = 0;
	std::size_t end_of_paragraph;
	while ((end_of_paragraph = input.find('\n', start_of_paragraph)) != std::string::npos)
	{
		std::size_t start_of_next_paragraph = input.find_first_not_of('\n', end_of_paragraph);
		if (start_of_next_paragraph == std::string::npos) start_of_next_paragraph = input.size();
		result_string.append(input.substr(start_of_paragraph, end_of_paragraph - start_of_paragraph));
		result_string.append("</p>" + std::string(start_of_next_paragraph - end_of_paragraph - 1, '\n') + "<p>");
		start_of_paragraph = start_of_next_paragraph;
	}
	result_string.append(input.substr(start_of_paragraph, std::string::npos));
	result_string.append("</p>");
	return result_string;
}


static struct suppress_history_t {} suppress_history;
static struct render_the_page {} r;
static struct aside_link_with_no_keyboard_shortcut {} a;
static struct neutral_link {} n; //neutral link gets a special shortcut
static struct no_automatic_spaces {} ns;

class html_output
{
	bool needs_whitespace = false;
	bool whitespace(std::string_view newstring)
	{
		if (newstring.empty()) return false;
		bool old_needs_whitespace = needs_whitespace;
		char no_whitespace_after[] = {'\r', '\n', '\t', '{', '[', '(', '#', '$'};
		char no_whitespace_before[] = {'@', '/', '\\', '?', '!', '.', ',', ';', ':', ')', ']', '}', '\'', '\"', '\r', '\n', '\t'};
		needs_whitespace = true;
		for (char n : no_whitespace_after)
			if (newstring.back() == n) needs_whitespace = false;
		if (!old_needs_whitespace) return false;

		for (char n : no_whitespace_before)
			if (newstring.front() == n) return false;
		return true;
	}
public:
	//any text appearing after this marker will appear in the message, but won't appear in the history section
	int suppress_history_section_number = -1;
	int suppress_history_char_offset;

	std::vector<std::string> texts{""};
	std::vector<std::pair<std::string, std::function<void(void)>>> links;
	unsigned main_links_emitted = 0;
	std::array<int, link_count + 1> link_position; //positions of the links with keybinds. last element is the neutral link

	html_output() { link_position.fill(-1); }

	//" << " takes 5 keystrokes including shift, and () takes 3 keystrokes when chaining as )(. operator() has the unique advantage of arbitrary argument count
	html_output& operator()(std::string_view text)
	{
		texts.back().append(whitespace(text) ? " " : "");
		texts.back().append(text);
		return *this;
	}

	//to_string('a') = "97", so we need a special case for chars
	html_output& operator()(char text) { return (*this)(std::string(1, text)); }

	//anything that supports std::to_string() is converted to string
	template<typename output_type>
	auto operator()(output_type const& f)
		-> decltype(std::to_string(f), *this)& //make sure to_string exists, and then use comma operator to return html_output&
	{ return (*this)(std::to_string(f)); }

	template<typename string_type>
	html_output& operator()(string_type const& text, no_automatic_spaces ns)
	{
		needs_whitespace = false;
		(*this)(text);
		needs_whitespace = false;
		return *this;
	}

	//aside links don't add any special properties.
	html_output& operator()(std::string_view text, std::function<void(void)> function, aside_link_with_no_keyboard_shortcut a)
	{
		if (whitespace(text)) texts.back().append(" ");
		links.emplace_back(std::make_pair(text, function));
		texts.emplace_back(""); //texts insert at next element
		needs_whitespace = true; //links shouldn't end in whitespace
		return *this;
	}

	html_output& operator()(std::string_view text, std::function<void(void)> function)
	{
		if (main_links_emitted < link_count)
			link_position.at(main_links_emitted++) = links.size();
		return (*this)(text, function, a); //use the aside link codepath
	}
	html_output& operator()(std::string_view text, std::function<void(void)> function, neutral_link n)
	{
		if (link_position.at(link_count) != -1)
			(*this)("[Warning: neutral command already set]");
		link_position.at(link_count) = links.size();
		return (*this)(text, function, a); //use the aside link codepath
	}

	html_output& operator()(suppress_history_t s)
	{
		if (suppress_history_section_number != -1)
			(*this)("[Warning: suppressing history twice in a single passage]");
		suppress_history_section_number = texts.size() - 1;
		suppress_history_char_offset = texts.back().size();
		return *this;
	}

	//return void to prevent further chaining, because clearing the screen and then adding more text is likely a bug.
	void operator()(render_the_page r)
	{
		int main_link_incrementor = 0;

		std::string output_string;
		for (int increm = 0; ; ++increm)
		{
			if (increm >= texts.size()) break;
			output_string.append(texts.at(increm));

			if (increm >= links.size()) break;

			int id_number = -1; //default: no keyboard shortcut
			if (link_position.at(link_count) == increm) id_number = link_count; //neutral link shortcut
			else if (link_position.at(main_link_incrementor) == increm) id_number = main_link_incrementor++; //keyboard shortcut

			//we need to insert [] here. because if we inserted it using the ::before and ::after pseudo-elements, we wouldn't be able to have subscripts. that's because <sub> is not allowed in pseudo-elements, so you have to style the whole pseudo-element together, and ] and the shortcut need different styles.
			if (links.at(increm).second == nullptr)
				output_string.append("<a class=disabled_link>[" + links.at(increm).first + "]</a>");
			else
			{
				//note: hard to use onmousedown='!event.button && Module._i(3)' to capture left clicks only, because keyboard enter doesn't set an event.
				std::string link_preamble("<a onmousedown='Module._i(" + std::to_string(increm) + ")' tabindex=0"); //set tabindex here so it can be modified
				if (id_number != -1) link_preamble.append(" id=l" + std::to_string(id_number));
				output_string.append(link_preamble + ">[" + links.at(increm).first + "]</a>");
			}
		}

		//theoretically, we must convert newlines before converting links to <a>, because </p><p> inside an <a> is disallowed in html. but IE, Chrome, and FF don't care. each link component on each line gets [] wrapped around it, rather than [] wrapped just once around the entire link, but that's ok.
		//the cost of doing things correctly, by converting links afterwards, would require tracking the history suppression marker. I did that for a while, but then removed that code.
		output_string = convert_newlines_to_paragraph_tags(output_string);

		extern std::string history_string;

		if (history_string.empty()) EM_ASM_({new_message($0)}, output_string.c_str());
		else EM_ASM_({insert_history($0); new_message($1)}, history_string.c_str(), output_string.c_str());

		history_string.clear();
	}
};
extern html_output o;

#pragma once
//o("text") to add text. 
//o("link text", function_to_call_on_click) to add links
//o(r) to flush your text to the screen.
//chainable: o("first")("second")("link", function)(r).

//o("text", ns) to disable the automatic spacing before the text.
//o("text", function, n) to add the unique neutral link.
//o("text", function, a) to add an aside link, which doesn't have a keyboard shortcut.
//o(suppress_history) to prevent the text afterward from appearing in the scrollback history.

#include <string>
#include <vector>
#include <functional>
#include <cstdlib>
#include <array>
#include <emscripten/emscripten.h>

[[noreturn]] inline void error(std::string message)
{
	message.insert(0, "Error: ");
	EM_ASM_({alert(Pointer_stringify($0));}, message.c_str());
	std::abort();
}
inline void warning(std::string message)
{
	message.insert(0, "Warning: ");
	EM_ASM_({alert(Pointer_stringify($0));}, message.c_str());
}
constexpr int link_count = 8;

//turn \n into paragraphs to improve accessibility and formatting
inline void convert_newlines_to_paragraph_tags_in_place(std::string& input)
{
	std::size_t pos = 0;
	while ((pos = input.find('\n', pos)) != std::string::npos) input.replace(pos, 1, "</p><p>");

	//sequential substitutions cause empty paragraphs, \n\n -> </p><p></p><p> -> </p>\n<p>
	pos = 0;
	while ((pos = input.find("<p></p>", pos)) != std::string::npos) input.replace(pos, 7, "\n");
}

extern std::string history_string;

//special types used for parameter matching functions in html_output
static struct suppress_history_t {} suppress_history;
static struct render_the_page {} r;
static struct aside_link_with_no_keyboard_shortcut {} a;
static struct neutral_link {} n; //neutral link gets a special shortcut
static struct no_automatic_space_before_this_text {} ns;
class html_output
{
	bool needs_whitespace; //this is true if the last output ended in a tab or newline
	bool whitespace(std::string const& newstring)
	{
		bool old_needs_whitespace = needs_whitespace;
		if (!newstring.empty())
			needs_whitespace = ((newstring.back() != '\n') && (newstring.back() != '\t')); //we don't check for ' ' to respect the user's intention of having an extra space.
		else return false;
		if (!old_needs_whitespace) return false;
		switch (newstring.front())
		{
		case '?':
		case '!':
		case '@':
		case '/':
		case '.':
		case ',':
		case ';':
		case ':':
		case ')':
		case ']':
		case '}':
		case '\'':
		case '\"':
		case ' ':
		case '\r':
		case '\n':
		case '\t':
			return false;
		default:
			return true;
		}
	}
public:
	//any text appearing after this marker will appear in the message, but won't appear in the history section
	int suppress_history_section_number;
	int suppress_history_char_offset;


	std::vector<std::string> texts;
	std::vector<std::pair<std::string, std::function<void(void)>>> links;
	unsigned main_links_emitted;
	std::array<int, link_count + 1> link_position; //positions of the links with keybinds. last element is the neutral link

	html_output() : needs_whitespace(false), suppress_history_section_number(-1), texts{""}, main_links_emitted(0)
	{
		link_position.fill(-1);
	}

	//" << " takes 5 keystrokes including shift, and () takes 3 keystrokes when chaining as )(. operator() has the unique advantage of arbitrary argument count
	html_output& operator()(std::string const& text)
	{
		texts.back().append((whitespace(text) ? " " : "") + text);
		return *this;
	}

	//to_string('a') = "97", so we need a special case for chars
	html_output& operator()(char text) { return (*this)(std::string(1, text)); }

	//anything that supports std::to_string() is converted to string
	template<typename output_type>
	auto operator()(output_type const& f)
		-> decltype(std::to_string(f), *this)& //make sure to_string exists, and then use comma operator to return html_output&
	{return (*this)(std::to_string(f)); }

	template<typename string_type>
	html_output& operator()(string_type const& text, no_automatic_space_before_this_text ns)
	{
		needs_whitespace = false;
		return (*this)(text);
	}

	//aside links don't add any special properties.
	html_output& operator()(std::string const& text, std::function<void(void)> function, aside_link_with_no_keyboard_shortcut a)
	{
		if (whitespace(text)) texts.back().append(" ");
		links.emplace_back(std::make_pair(text, function));
		texts.emplace_back(""); //add an empty element
		needs_whitespace = true; //because the [e] comes after the link
		return *this;
	}

	html_output& operator()(std::string const& text, std::function<void(void)> function)
	{
		if (main_links_emitted < link_count)
			link_position.at(main_links_emitted++) = links.size();
		return (*this)(text, function, a); //use the aside link codepath
	}
	html_output& operator()(std::string const& text, std::function<void(void)> function, neutral_link n)
	{
		if (link_position.at(link_count) != -1)
			error("neutral command already set");
		link_position.at(link_count) = links.size();
		return (*this)(text, function, a); //use the aside link codepath
	}

	html_output& operator()(suppress_history_t s)
	{
		if (suppress_history_section_number != -1)
		{
			warning("suppressing history twice in a single passage");
			return *this; //already suppressing history
		}
		suppress_history_section_number = texts.size() - 1;
		suppress_history_char_offset = texts.back().size();
		return *this;
	}

	//return nothing, because clearing the screen and then adding more text is likely not the desired behavior.
	void operator()(render_the_page r)
	{
		int main_link_incrementor = 0;

		std::string output_string("<p>");
		for (int increm = 0; ; ++increm)
		{
			if (increm >= texts.size()) break;
			output_string.append(texts.at(increm));
			
			if (increm >= links.size()) break;

			int id_number = -1; //default: no keyboard shortcut
			if (link_position.at(link_count) == increm) id_number = link_count; //neutral link shortcut
			else if (link_position.at(main_link_incrementor) == increm) id_number = main_link_incrementor++; //keyboard shortcut

			//if (links.at(increm).first.empty()) continue; //don't output a link, because the user might have keyboard input turned off? but some games might want keyboard-only input. however, the game author should be aware that this is not a mouse-friendly action.
			if (links.at(increm).second == nullptr)
				output_string.append("<a class='disabled_link'>" + links.at(increm).first + "</a>");
			else
			{
				std::string link_preamble("<a onclick = 'Module._i(" + std::to_string(increm) + ");' tabindex='0'"); //set tabindex here so it can be modified
				if (id_number != -1) link_preamble.append(" id='l" + std::to_string(id_number) + "' + class='l" + std::to_string(id_number) + "'>");
				else link_preamble.append(">"); //generic link without a keyboard bind.
				output_string.append(link_preamble + links.at(increm).first + "</a>");
			}
		}
		output_string.append("</p>"); //append this before conversion, because last line might be '<p></p>'

		//theoretically, we must convert newlines before converting links to <a>, because inserting </p><p> inside an <a> is disallowed in html. but IE, Chrome, and FF don't care. each link component on each line gets [] wrapped around it, rather than [] wrapped just once around the entire link, but that's ok.
		//the cost of doing things correctly, by converting links afterwards, would require tracking the history suppression marker. I did that for a while, but then removed that code.
		convert_newlines_to_paragraph_tags_in_place(output_string);

		if (history_string.size() <= 1) EM_ASM_({change_message($0)}, output_string.c_str()); //either empty string or \n, which is created by empty string
		else EM_ASM_({insert_history($0); change_message($1)}, history_string.c_str(), output_string.c_str());

		history_string.clear();
	}
};
extern html_output o;

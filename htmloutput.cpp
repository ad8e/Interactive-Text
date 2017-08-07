#include "htmloutput.h"

std::string history_string;
html_output o;

extern "C" {
	void EMSCRIPTEN_KEEPALIVE i(int chosen_link) //if clicked link, count up from 0 sequentially. keyboard press, count down from -1 sequentially, with neutral link = -link_count - 1.
	{
		int link_in_order;
		if (chosen_link >= 0) link_in_order = chosen_link; //mouse clicked
		else //chosen_link < 0, which means keyboard pressed
		{
			link_in_order = o.link_position.at(-chosen_link - 1); //either a non-negative number or -1
			if (link_in_order == -1) return;
		}

		if (link_in_order >= o.links.size()) return; //referenced a link which doesn't exist
		if (o.links.at(link_in_order).second == nullptr) return; //if the link is nullptr, it's a dummy link

		//reconstruct the message without clickable links.
		//we can't move the existing DOM message to the history section, because we'll have a nesting error if the </span> of the cmessage is in the middle of a paragraph.
		//if we did move something, we could use http://stackoverflow.com/a/33724397, which moves them to a fragment before moving them again. operating on non-visible elements is faster.

		for (int increm = 0; ; ++increm)
		{
			if (increm >= o.texts.size()) break;
			if (o.suppress_history_section_number == increm)
			{
				history_string.append(o.texts.at(increm).substr(0, o.suppress_history_char_offset));
				break;
			}
			history_string.append(o.texts.at(increm));

			if (increm >= o.links.size()) break;
			if (o.links.at(increm).second == nullptr) history_string.append("<a class='disabled_link'>" + o.links.at(increm).first + "</a>");
			else history_string.append(((increm == link_in_order) ? "<a class='history chosen_link_in_history'>" : "<a class = 'history'>") + o.links.at(increm).first + "</a>");
		}
		history_string = convert_newlines_to_paragraph_tags(history_string);

		std::function<void()> to_be_called = o.links.at(link_in_order).second;
		o = html_output(); //reset it
		to_be_called();
	}
}

//we initialize emscripten in a static initializer, instead of in main, to keep the user-modified file small.
//this should be ok, because JS is single-threaded, and I don't think there is anything run between static initialization and main()
const int initialize_emscripten = []{EM_ASM(emscripten_loaded = true;); return 0;}();
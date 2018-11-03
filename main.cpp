#include "htmloutput.h"

void main_menu_later();
void engine_choice();
void emscripten_notes() {
	o(R"(Interactive Text is just output and formatting. Emscripten does the real work converting C++ to Javascript. Emscripten has some catches.
Saving is easiest through localStorage. If you want to save binary data, use base64. If you really need filesystem read/write, you have to add some emscripten sync functions, and you need --preload-file if you want to load pre-existing files.
Keyboard input handling uses Javascript's onkeypress, which only supports keys with concrete glyphs, like "a" and "=". To support keys like "Ctrl" and arrow keys, you'll need onkeydown, number-to-symbol tables for each browser, and browser sniffing, which is annoying.
When testing in IE, if you're running the html file on your computer instead of a server, you'll need to follow emscripten's <a href="https://kripken.github.io/emscripten-site/docs/getting_started/Tutorial.html#using-files">guide on setting up a localhost web server for development</a>. This is because IE disallows localStorage for local files.
Add '\n' to your console output messages, or they won't show up. Use EM_ASM(console.log("text");) instead of iostreams, because cout has a <a href="https://floooh.github.io/2016/08/27/asmjs-diet.html">large fixed cost</a>, 250KB.
)");
	o("Return", engine_choice, n)(r);
}

void other_engines() {
	o(R"(Java has severe UI and performance problems in every text game I've seen.
Ren'Py is powerful, mature, and well-supported; it is good if you want to build a Visual Novel.
IF parser engines, like Inform, place restrictions on your format and require a language you will never use anywhere else. They're decent within their format, but most players will refuse to play them.
RAGS, QSP, and Quest are dumpster fires.
Adobe Flash is dead.
Unity has some formatting, and it can produce HTML files, but for a text game, it's too heavy and proprietary. Starting up takes significant work, with no benefit. You'll be re-implementing another browser inside your browser.
HTML is best for a text game. It has colors, fonts, sectioned displays, buttons, and pictures. Its wide developer support makes it supreme in performance and flexibility. It is multiplatform. Most importantly, people play browser games, but most people won't play downloaded games.
For any minor engine, be aware that if its developer vanishes, your work could become useless. Even if the engine is open-source, it's unlikely anyone else will step in. Interactive Text faces this same issue, and tries to be small so that it's easy to modify. However, you should still be mindful.
)");
	o("Return", engine_choice, n)(r);
}

void engine_choice() {
	o(R"(Technical details:
<ul>
<li>lightweight and performant: this webpage is 52KB to download</li>
<li>line spacing and margin adjust to screen size</li>
<li>link format avoids empty history entries and empty new tabs on middle click</li>
<li>supports screenreaders for visually impaired users</li>
<li>tested in Firefox, Chrome, and IE</li>
<li>HTML makes customization easy</li>
<li>scrollback history and keyboard shortcuts: click the bottom options to try them.</li>
</ul><h4>How do I build a text game?</h4>
<a href="https://twinery.org/">Twine</a> is the best choice for most. It lets you write without programming. It has built-in saves. It allows undo in-game. Its editor is annoying and laggy, but it exists. Gameplay past 10 hours suffers from poor performance; each click freezes for several seconds.
Interactive Text is a niche alternative for games which prefer C++ to Javascript. It has an std::cout-a-like and link callbacks. It doesn't have built-in saves.
Both options create webpages as output.
)");
	o("Other engines", other_engines)('\n');
	o("Notes on the compiler", emscripten_notes)('\n');
	o("Return to the main menu", main_menu_later, n)(r);
}

int incrementor = 0;
void guestbook_incrementor() {
	o("You doodle on the board.");
	++incrementor;
	main_menu_later();
}
void erase_incrementor() {
	o("You wipe the board clean.\n");
	incrementor = 0;
	main_menu_later();
}

void main_menu_later() {
	if (incrementor != 0) {
		o("Draw on it again?", guestbook_incrementor)("Or")("erase the board?", erase_incrementor);
		o(R"(<div style="text-align:center;">)");
		for (int start = incrementor; start; --start) o(R"(<img src="Fractal_fern_explained.png" alt="doodle" style="width:333px; height:465px">)");
		o("</div>");
	}
	o(R"(
<a href="https://github.com/ad8e/Interactive-Text">Interactive Text</a> turns C++ text games into HTML. Features:
<ul>
<li>simple: o() to output</li>
<li>Public Domain: open source, no conditions</li>
<li>this webpage was built with it</li>
</ul>
)");

	o("Further details", engine_choice)('\n');

	if (incrementor == 0) o("In front of you is a board.")("Draw on it?", guestbook_incrementor)('\n');
	o(R"(<a href="https://github.com/ad8e/Interactive-Text">Download the engine.</a>)");
	o(r);
}

int main() {
	o(R"(<link rel="prefetch" href="Fractal_fern_explained.png">)");
	main_menu_later();
}
#include "o.h"

void main_menu_later();
void engine_choice();
void emscripten_notes() {
	o(R"(Interactive Text is just output and formatting. Emscripten does the real work converting C++ to Javascript. Emscripten has some catches.
Use localStorage for saves. To save binary data, use base64.
Keyboard input uses Javascript's onkeypress, which only supports keys with concrete glyphs, like "a" and "=". To support keys like "Ctrl" and arrow keys, you'll need onkeydown, number-to-symbol tables for each browser, and browser sniffing, which is annoying.
<a href="https://kripken.github.io/emscripten-site/docs/compiling/Running-html-files-with-emrun.html">When testing in Internet Explorer, set up a localhost web server</a>. This is because IE disallows localStorage for local files.
Add '\n' to your console output messages, or they won't show up. <a href="https://floooh.github.io/2016/08/27/asmjs-diet.html">Don't use iostreams; they are expensive</a>, 250KB.
)");
	o("Return", engine_choice, n)(r);
}

void other_engines() {
	o(R"(Java has severe UI and performance problems in every text game I've seen.
Ren'Py is powerful, mature, and well-supported; it is good if you want to build a Visual Novel.
IF parser engines, like Inform, place restrictions on your format and require a language you will never use anywhere else. They're decent within their format, but most players will refuse to play them.
RAGS, QSP, and Quest are dumpster fires.
Adobe Flash is dead.
Unity has some formatting and can produce webpages. But for a text game, you'll be re-implementing another browser inside a browser. Performance and quality of your implementation will suck.
HTML is best for a text game. It has colors, fonts, sectioned displays, buttons, and pictures. Its wide developer support makes it supreme in performance and flexibility. It is multiplatform. Most importantly, people play browser games, but most people won't play downloaded games.
For any minor engine, be aware that if its developer vanishes, your work could become useless. Even if the engine is open-source, it's unlikely anyone else will step in. Interactive Text faces this same issue, and tries to be small so that it's easy to modify. However, you should still be mindful.
)");
	o("Return", engine_choice, n)(r);
}

void engine_choice() {
	o(R"(Technical details:
<ul style=margin:0>
<li>lightweight and performant: this webpage is 52KB to download</li>
<li>line spacing and margin adjust to screen size</li>
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

int incrementor = 299; //odd = erased, even = draw
void guestbook_incrementor() {
	o("You doodle on the board.");
	incrementor = (incrementor) / 2 * 2 + 2;
	main_menu_later();
}
void erase_incrementor() {
	o("You wipe the board clean.\n");
	++incrementor;
	main_menu_later();
}

void main_menu_later() {
	static int lookahead = 2;
	preload_next_picture:
	std::string next_pic = "https://picsum.photos/" + std::to_string(incrementor / 2 * 2 + lookahead) + "/200/?random";
	EM_ASM_({(new Image()).src=UTF8ToString($0)}, next_pic.c_str());
	if (incrementor == 304 && lookahead == 2) { //user clicked on three pictures. so preload the next two pictures
		lookahead = 4;
		goto preload_next_picture;
	}

	if (!(incrementor & 1)) {
		o("Draw on it again?", guestbook_incrementor)("Or")("erase the board?", erase_incrementor);
		o(R"(<div style="text-align:center;"><img src="https://picsum.photos/)")(incrementor, ns)(R"(/200/?random" alt=doodle style="width:)")(incrementor, ns)(R"(px; height:200px"></div>
)");
	}
	o(R"(Interactive Text turns C++ text games into HTML. Features:
<ul style=margin:0>
<li>simple: o() to output</li>
<li>Public Domain: open source, no conditions</li>
<li>this webpage was built with it</li>
</ul>
)");

	o("Further details", engine_choice)('\n');

	if (incrementor & 1) o("In front of you is a board.")("Draw on it?", guestbook_incrementor)('\n');
	o(R"(<a href="https://github.com/ad8e/Interactive-Text">Download the engine.</a>)");
	o(r);
}

int main() {
	//o(R"(<link rel="preload" href="Fractal_fern_explained.png">)", ns); //seems to do nothing
	main_menu_later();
}
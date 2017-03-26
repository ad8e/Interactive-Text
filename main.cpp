#include "htmloutput.h"

void main_menu();

void engine_choice();
void emscripten_notes()
{
	o(R"(Interactive Text is just an output function. Emscripten does the heavy lifting, converting the C++ to HTML. There are some catches.
For filesystem read/write, you have to add some emscripten sync functions if you want saves, and you need --preload-file if you want to load pre-existing files. Alternatively, you can use html's localStorage for saves.
Keyboard input handling is through Javascript, using onkeypress, which only supports keys with concrete glyphs, like "a" and "=". To support keys like "Ctrl" and arrow keys, you'll need onkeydown, number-to-symbol tables for each browser, and browser sniffing.
When testing in Chrome and IE, if you're running the html file on your computer instead of a server, you'll need to follow emscripten's <a href="https://kripken.github.io/emscripten-site/docs/getting_started/Tutorial.html#using-files">guide on setting up a localhost web server for development</a>. Firefox doesn't need this step.
If your std::cout messages aren't showing in the console log, add a '\n' to the end. Use cout instead of cerr, cerr doesn't seem to show up. But try to use printf instead of cout, because cout adds a <a href="https://floooh.github.io/2016/08/27/asmjs-diet.html">large fixed cost</a>, about 250KB to the .js for this engine.
)");
	o("Return", engine_choice, n)(r);
}

void other_engines()
{
	o(R"(Java has severe UI and performance problems in every text game I've seen.
Ren'Py is powerful, mature, and well-supported; it is good for Visual Novels. If you want to build a Visual Novel, Ren'Py is king.
Old-style IF parser engines, like Inform, place restrictions on your format and require you to learn a language you will never use anywhere else. If you like the format, they're not a bad choice, but most people don't. The IF audience is small.
RAGS, QSP, and Quest are dumpster fires.
Adobe Flash is dying. You can still build with it, but its future is limited.
Unity has some formatting, and it can output to HTML, but for a text game, it's too heavy and proprietary. Setting up a basic product takes a lot of work.
HTML is best for a text game. It has colors, fonts, bold text, sectioned displays, buttons, and pictures. Its huge developer base ensures its supremacy in performance and flexibility. It is multiplatform. You won't have to learn engine-specific features that will never be useful anywhere else. Most importantly, people play browser games, but most people won't play downloaded games.
For minor engines, be aware that if the developer vanishes into smoke, you could be left holding a useless file. Even if the engine is open-source, it's unlikely anyone else will step in. Interactive Text faces this same issue, and tries to be small so that it's easy to modify. However, you should still be mindful.
)");
	o("Return", engine_choice, n)(r);
}

void engine_choice()
{
	o(R"(<h4>How do I build a text game?</h4>Your main options are Twine and Interactive Text.
Twine is a strong choice. It has built-in saves. It has undo-action capabilities. It publishes to HTML. You can start writing immediately. Its editor is annoying and laggy, but tolerable. Programming is in Javascript. However, games longer than 10 hours become unplayable from lag. Each choice freezes the browser for several seconds, and restarting the browser doesn't help. If your game is less than three hours long, Twine's built-in features are amazing. For longer projects, Twine is unusable.
Interactive Text is an alternative that doesn't have lag problems, but if your game runs over 10 hours, your users will need a save function, which you will need to build. Twine has saves, but Interactive Text doesn't. Interactive Text requires knowledge of C++ instead of Javascript. Your finished product will be an HTML file that can be run in a browser. Installing the C++-to-HTML compiler takes a while; 30 minutes of reading and an hour of waiting.

)");
	o("Notes on emscripten", emscripten_notes)('\n');
	o("Other engines", other_engines)('\n');
	o("Return to the main menu", main_menu, n)(r);
}


void getting_started()
{
	o(R"(1. install <a href="https://kripken.github.io/emscripten-site/docs/getting_started/downloads.html">emscripten</a>, the compiler that does most of the work
2. download <a href="https://github.com/ad8e/Interactive-Text">this project</a>
3. open a console in the project folder (on Windows, shift right-click when nothing is selected), and compile by pasting this command:
emcc htmloutput.cpp sample.cpp --shell-file shell.html -o index.html -std=c++1z -s NO_EXIT_RUNTIME=1 -O3 --closure 1
4. run index.html to test the compiled files. Don't use Internet Explorer for this step.
5. to start writing, modify sample.cpp, and compile again to see your changes.
)");
	o("Return to the main menu", main_menu, n)(r);
}

void main_menu_later();
int incrementor = 0;
void guestbook_incrementor() {
	o("You doodle on the board.");
	++incrementor;
	main_menu_later();
}
void erase_incrementor() {
	o("You wipe the board clean.");
	incrementor = 0;
	main_menu_later();
}
void main_menu() {
	if (incrementor == 0) o("In front of you is a board.");
	main_menu_later();
}

void main_menu_later() {
	if (incrementor == 0) o("Draw on it?", guestbook_incrementor);
	else
	{
		o("Draw on it again?", guestbook_incrementor)("Or")("erase the board?", erase_incrementor);
		o(R"(<div style="text-align:center;">)");
		for (int start = incrementor; start; --start) o(R"(<img src="Fractal_fern_explained.png" alt="doodle" style="width:333px; height:465px">)");
		o("</div>\n");
	}
	o(R"(
Features:
<ul>
<li>only one function to learn: o() to output</li>
<li>scrollback history</li>
<li>keyboard shortcuts for links</li>
<li>code in C++</li>
<li>Public Domain license: no restrictions on use</li>
</ul>
Technical details:
<ul>
<li>line spacing and margin adjusts to window size</li>
<li>snag-free link format avoids empty history entries and empty new tabs on middle click</li>
<li>roles, semantic tags, and help section for screenreader users</li>
<li>tested in IE, Firefox, and Chrome</li>
<li>on desktop browsers, asm.js means unmatched performance</li>
<li>mobile browsers supported with much less performance, but should still be fine</li>
<li>entire display is in HTML, so customization is easy</li>
</ul>
)");

	o("Build your own game", getting_started)("\n")("Receive advice on engine choice", engine_choice)(r);
}

int main() {
	main_menu();
}
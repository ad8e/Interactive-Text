#include "htmloutput.h"

void main_menu();
void tech_demo();

void feature_list()
{
	o(R"A(Features:
can be learned in one minute, because there's only one function: o() to output
scrollback history
rebindable keyboard shortcuts for links
support for low-vision users, keyboard-only users, mouse-only users
fully performant and responsive, vastly better than any Javascript engine
code in C++, no need to use Javascript
Public Domain license: you can use this code as if you wrote it yourself
HTML output makes customizing the display easy; stackoverflow has every answer.

Technical details:
sophisticated line spacing and margin management
perfect Javascript link format: no empty history entries, no empty new tabs on middle click
roles, semantic tags, and help section for screenreader users
tested in IE, Firefox, and Chrome
on desktop browsers, asm.js and emscripten eliminate garbage collection and dynamic typing, receiving stutter-free 50%-native performance
)A");
	o("Return to the main menu", main_menu, n)(r);
}

void engine_choice();
void emscripten_notes()
{
	o(R"(This engine is just an output function. Emscripten does the heavy lifting. There are some catches.
For filesystem read/write, you have to add some emscripten sync functions if you want saves, and you need --preload-file if you want to load pre-existing files. Alternatively, you can use html's localStorage.
Keyboard input handling is through Javascript, using "onkeypress", which only supports keys with concrete glyphs like "a" and "=". To support other keys, you'll need "onkeydown", number-to-symbol tables for each browser, and browser sniffing.
When testing in Chrome and IE, if you're running the html file on your computer instead of a server, you'll need to follow emscripten's <a href="https://kripken.github.io/emscripten-site/docs/getting_started/Tutorial.html#using-files">guide on setting up a localhost web server for development</a>. Firefox doesn't need this step.
If your std::cout messages aren't showing in the console log, add a '\n' to the end. Use cout instead of cerr, cerr doesn't seem to show up. But try to use printf instead of cout, because cout adds a <a href="https://floooh.github.io/2016/08/27/asmjs-diet.html">large fixed cost</a>, about 250KB to the .js for this engine.
)");
	o("Return", engine_choice, n)(r);
}

void other_engines()
{
	o(R"(Java is laggy and its UI is buggy.
Ren'Py is powerful, mature, and well-supported, but its strength is Visual Novels.
Old-style IF parser engines, like Inform, place restrictions on your format and require you to learn their language.
RAGS, QSP, and Quest are dumpster fires.
Adobe Flash is dying.
Unity has some formatting, and it can output to HTML, but for a text game, it's too heavy and proprietary a dependency to set up and carry.
HTML is the best format for a text game. Your browser has features like fonts, bold text, sectioned displays, buttons, and pictures, and its hundreds of developers ensure that no other text engine comes close in performance and flexibility. HTML is multiplatform. You won't have to learn engine-specific features, and the engine won't go obsolete. Most importantly, being able to run your game in a browser causes a large boost to your audience, setting HTML above every other choice.
)");
	o("Return", engine_choice, n)(r);
}

void engine_choice()
{
	o(R"(<h4>How do I build a text game?</h4>Your main options are Twine and this engine.
Twine is a strong choice. It has built-in saves. It has undo-action capabilities. It publishes to HTML. You can start writing immediately. Its editor is annoying and laggy, but tolerable. Programming is in Javascript. However, games longer than 10 hours become unplayable from lag. Each choice freezes the browser for several seconds, and restarting the browser doesn't help. If your game is less than three hours long, Twine's built-in features are amazing. For longer projects, Twine is unusable.
This engine lets C++ run in a webpage, thanks to emscripten. You don't need to know Javascript or HTML. This avoids Twine's lag, but if your game runs over 10 hours, your users will need a save function. Twine has saves, but this engine doesn't. You will have to build your own save function.

)");
	o("Notes on emscripten", emscripten_notes)('\n');
	o("Other engines", other_engines)('\n');
	o("Return to the main menu", main_menu, n)(r);
}


void getting_started()
{
	o(R"(To get started,
1. install <a href="https://kripken.github.io/emscripten-site/docs/getting_started/downloads.html">emscripten</a>, the compiler that does most of the work
2. download <a href="https://github.com/ad8e/Text-Game-Standard-Engine">this project</a>
3. open a console in the project folder (on Windows, shift right-click when nothing is selected), and compile by pasting this command:
emcc htmloutput.cpp sample.cpp --shell-file shell.html -o index.html -std=c++1z -s NO_EXIT_RUNTIME=1 -O3 --closure 1
4. run index.html to test the compiled files. Don't use Internet Explorer for this step.
5. to start writing, modify sample.cpp, and compile again to see your changes.
)");
	o("Return to the main menu", main_menu, n)(r);
}

int incrementor = 0;
void guestbook_incrementor() {
	o("Gripping a brush, you pour your soul onto the unsuspecting board.\n");
	++incrementor;
	tech_demo();
}
void erase_incrementor() {
	o("You wipe the board free of the offending marks.\n");
	incrementor = 0;
	tech_demo();
}

void tech_demo() {
	o("In front of you is a board");
	if (incrementor == 0) o(".");
	else
	{
		if (incrementor == 1) o("which has been defaced with a shoddy doodle.");
		else o("which currently has")(incrementor)("amateur scribbles.");
	}
	o("\n")("Add a beautiful drawing?", guestbook_incrementor);
	if (incrementor) o("\n")("Or erase the board?", erase_incrementor);
	o(suppress_history)("\nThe last link is useful for a")("back button", main_menu, n)("or continue button.\nIn typography, line spacing depends on line length, so the space between lines will adjust as you rescale your window.");
	o(R"(<div style="text-align:center;"><img src="Fractal_fern_explained.png" alt="image demo" style="width:333px; height:465px"></div><div style="text-align:right;">A centered image, and right-justified text.</span>)")(r);
}

void main_menu() {
	o("This webpage describes the Text Game Standard Engine using a stock version of it. You can choose links by clicking on them or pressing the keybind next to them.\n")("Feature list", feature_list)("\n")("Demo", tech_demo)("\n")("Build your own game", getting_started)("\n")("Receive advice on engine choice", engine_choice)(r);
}

int main() {
	main_menu();
}
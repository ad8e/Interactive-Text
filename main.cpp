#include "htmloutput.h"

void main_menu();
void tech_demo();

void feature_list()
{
	o(R"A(Features:
scrollback history
rebindable keyboard shortcuts for links
support for low-vision users, keyboard-only users, mouse-only users
fully performant and responsive, vastly better than any Javascript engine
code in C++, no need to use Javascript
every feature is removable
Public Domain license: you can use this code as if you wrote it yourself

Technical details:
only 600 lines of novel code for the entire engine
sophisticated line spacing and margin management
perfect Javascript link format: no empty history entries, no empty new tabs on middle click
roles, semantic tags, and help section for screenreader users
tested in IE, Firefox, and Chrome
on desktop browsers, asm.js and emscripten eliminate garbage collection and dynamic typing, receiving lag-free 50%-native performance
)A");
	o("Return to the main menu", main_menu, n)(r);
}

void emscripten_notes()
{
	o(R"(This engine relies on emscripten to do the heavy lifting.
You can use the usual C++ filesystem IO, but you have to add some emscripten sync functions if you want saves, and you need --preload-file if you want to load pre-existing files. Alternatively, you can use html's localStorage.
Keyboard input handling is through Javascript, using "onkeypress", which doesn't support keys that are missing actual glyphs. To support all keys, you'll need "onkeydown", number-to-symbol tables for each browser, and browser sniffing.
When testing in Chrome and IE, if you're running the html file off your computer, you'll need to follow emscripten's <a href="https://kripken.github.io/emscripten-site/docs/getting_started/Tutorial.html#using-files">guide on setting up a localhost web server for development</a>. Firefox doesn't need this step.
If your std::cout messages aren't showing in the console log, add a '\n' to the end. Use cout instead of cerr, cerr doesn't seem to show up. But try to use printf instead of cout, because cout adds a <a href="https://floooh.github.io/2016/08/27/asmjs-diet.html">large fixed cost</a>, about 250KB to the .js for this engine.
You'll eventually want to read the emscripten docs, especially the ones on optimization.
)");
	o("Return to the main menu", main_menu, n)(r);
}

void engine_choice()
{
	o(R"(<h4>How do I build a text game?</h4>Your main options are Twine and this engine.
Twine is a strong choice. It has built-in saves. It has undo-action capabilities. It publishes to HTML. You can start writing immediately. Its editor is annoying and laggy, but tolerable. Programming is in Javascript. However, Twine is limited by its performance for long games. After 10 hours of play, every click will have seconds of delay, and the game becomes unplayable. This seems to be a property of the gamestate, so restarting doesn't help. This length limitation is the make-or-break factor for Twine. If your game is less than three hours long, Twine's built-in features are amazing. For longer projects, Twine is unusable.
Java is laggy and its UI is buggy. Ren'Py is powerful, mature, and well-supported, but its strength is Visual Novels. Old-style IF engines, like Inform, place restrictions on your format and require you to learn their language. RAGS, QSP, and Quest are dumpster fires. Adobe Flash is dying. Unity has some formatting, and it can output to HTML, but for a text game, it's too heavy and proprietary a dependency to set up and carry.
This engine lets you write C++ code and get HTML. You don't need to know Javascript or HTML, but you need a beginner's knowledge of C++. Adding features like sidebars is as easy as searching "html sidebars" on stackoverflow, there aren't any engine-specific things to learn.
HTML is the best format for a text game. Your browser has features like fonts, bold text, sectioned displays, buttons, and pictures, and its hundreds of developers ensure that no other text engine comes close in performance and flexibility. HTML is multiplatform. You won't have to learn engine-specific features, and the engine won't go obsolete. Most importantly, being able to run your game in a browser causes a large boost to your audience, setting HTML above every other choice.

)");
	o("Notes on emscripten", emscripten_notes)('\n');
	o("Return to the main menu", main_menu, n)(r);
}

void getting_started()
{
	o(R"(To get started,
1. install <a href="https://kripken.github.io/emscripten-site/docs/getting_started/downloads.html">emscripten</a>, the compiler that does most of the work
2. download <a href="https://github.com/ad8e/Text-Game-Standard-Engine">this project</a>
3. open a console in the project folder (on Windows, shift right-click when nothing is selected), and compile by pasting this command:
emcc htmloutput.cpp sample.cpp --shell-file shell.html -o index.html -std=c++1z -s NO_EXIT_RUNTIME=1
4. run index.html to test the compiled files. Don't use Internet Explorer for this step.
5. to start writing, modify sample.cpp, and compile again to see your changes.
)");
	o("Return to the main menu", main_menu, n)(r);
}

int incrementor = 0;
void guestbook_incrementor() {
	o("Gripping a brush, you paint a masterpiece on the board.\n");
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
		if (incrementor == 1) o("which has been defaced with a shoddy doodle. Who would do such a thing?");
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
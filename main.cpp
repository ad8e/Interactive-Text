#include "htmloutput.h"

void main_menu();
void tech_demo();

void feature_list()
{
	o(R"A(Features:
scrollback history
rebindable keyboard shortcuts for links
support for low-vision users, keyboard-only users, mouse-only users
automatic typography, with sophisticated line-spacing variation and margin management
perfectly responsive, no lag
code in C++, no need to use Javascript
every feature is removable

Technical details:
only 600 lines of novel code for the entire engine
automatic conversion of newline to paragraphs for formatting
perfect javascript link format: no empty history entries, no empty new tabs on middle click
roles, semantic tags, and help section for screenreader users
tested in IE, Firefox, and Chrome (Opera, Safari untested but likely to work)
)A");
	o("Return to the main menu", main_menu, n)(r);
}

void emscripten_notes()
{
	o(R"(This engine relies on emscripten to do the heavy lifting.
You can use the usual C++ filesystem IO, but you have to add some emscripten sync functions if you want saves, and you need --preload-file if you want to load pre-existing files. Alternatively, you can use html's localStorage.
Keyboard input handling is through javascript, and it doesn't support all keys. To support all keys, you'll need "onkeydown", number-to-symbol tables for each browser, and some browser sniffing. This engine uses a different method, "onkeypress", which is much simpler, avoids the browser compatibility problems, and displays symbols perfectly, but it only supports keys that have actual glyphs.
If emscripten is complaining about being out of memory, google "emscripten TOTAL_MEMORY". You could also use ALLOW_MEMORY_GROWTH which fixes this problem but eliminates emscripten's main performance advantage. This problem will likely disappear when WebAssembly arrives. See <a href="https://groups.google.com/forum/#!topic/emscripten-discuss/09E_WEk193E">this discussion</a>.
When testing in Chrome and IE, if you're running the html file off your computer, you'll need to follow emscripten's <a href="https://kripken.github.io/emscripten-site/docs/getting_started/Tutorial.html#using-files">guide on setting up a localhost web server for development</a>. Firefox doesn't need this step.
If your std::cout messages aren't showing in the console log, add a '\n' to the end. Use cout instead of cerr, cerr doesn't seem to show up. But try to use printf instead of cout, because cout adds a <a href="https://floooh.github.io/2016/08/27/asmjs-diet.html">large fixed cost</a>, about 250KB to the .js for this engine.
You'll eventually want to read the emscripten docs, especially the ones on optimization.
)");
	o("Return to the main menu", main_menu, n)(r);
}

void engine_choice()
{
	o(R"(<h4>How do I build a text game?</h4>Your main options are Twine and this engine.
Twine is a strong choice. It has built-in save-anywhere. It has undo-action capabilities. It publishes to HTML. You can start writing immediately. Its editor is annoying and laggy, but tolerable. Programming is in javascript. However, Twine is limited by its performance for long games. After 10 hours of play, every click will have seconds of delay, and the game becomes unplayable. This seems to be a property of the gamestate, so restarting doesn't help. This length limitation is the make-or-break factor for Twine. If your game is less than three hours long, Twine's built-in features are amazing. For longer projects, Twine cannot be used.
Java is laggy and has constant UI annoyances. Ren'Py is powerful, mature, and well-supported, but its strength is Visual Novels. Old-style IF engines, like Inform, place heavy restrictions on your format. RAGS, QSP, and Quest are dumpster fires. Adobe Flash is dying. Unity has some formatting, and it can output to HTML, but for a text game, it's too heavy and proprietary a dependency to set up and carry.*
HTML is by far the best way to display text. Your browser has hundreds of developers working on features like fonts, bold text, sectioned displays, buttons, and pictures. No other text engine comes close to your browser in performance and flexibility. HTML is present on Windows, Linux, Mac, and mobile. You won't have to learn engine-specific features, and you won't have to worry about the engine developer disappearing. Most importantly, many users are unwilling or unable to download things, and being able to run your game in a browser causes a large boost to your audience, setting HTML above every other choice.
This engine lets you write C++ code and get HTML. It's designed to be trivial to pick up. You don't need to know javascript or HTML, but you need a beginner's knowledge of C++. Adding features like sidebars is as easy as searching "html sidebars" on stackoverflow.

*The mandatory splash screen for free users is a big downside. "Powered by Unity" could be interpreted as expressing pride in your choice, but "Unity Personal Edition" is just making fun of your poverty and forcing you to tell everyone else.

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
emcc htmloutput.cpp sample.cpp --shell-file shell.html -o index.html --bind -s RESERVED_FUNCTION_POINTERS=1 -std=c++1z -s NO_EXIT_RUNTIME=1
4. run index.html to test the compiled files. Don't use Internet Explorer for this step.
5. to start writing, modify sample.cpp, and compile again to see your changes.

To output text from C++, write o("your text here");
To output a link, write o("click me", function_to_call_when_clicked);
To render the page, write o(r);
That's all you need to learn to start writing. If later, you see other features you like, come back to this tech demo to see its source code.
)");
	o("Return to the main menu", main_menu, n)(r);
}

int incrementor = 2;
void guestbook_incrementor() {
	++incrementor;
	tech_demo();
}


void tech_demo() {
	o("This")("link", tech_demo)("was produced by writing o(\"link\", tech_demo) in C++, and it calls the function tech_demo() when pressed, which happens to be this passage you're already in.")("This link is disabled", nullptr)(".")("This link has no keybind, and therefore you can tab to it", tech_demo, a)(". Disabled links consume keybinds by default, but")("this disabled link has no keybind", nullptr, a)(". This is useful for keeping buttons in consistent keyboard order when some of them are missing.\n");
	o("You see a board which was implemented in C++. It currently has")(incrementor)("amateur scribbles.")("Add a beautiful drawing", guestbook_incrementor)(".");
	o(suppress_history)("\nThis text will appear in the main message, but not in the history log. There is an optional neutral link, which has a keybind out of order. This is useful for a")("back button", main_menu, n)("or continue button, and is bound to spacebar. Its harmlessness is what makes it \"neutral\". \nIn typography, line spacing depends on line length, so the space between lines will adjust as you rescale your window.");
	o(R"(<div style="text-align:center;"><img src="Fractal_fern_explained.png" alt="image demo" style="width:333px; height:465px"></div><div style="text-align:right;">A centered image, and right-justified text.</span>)")(r);
}

void main_menu() {
	o("What would you like to do?")("See the feature list", feature_list)(",")("see a tech demo", tech_demo)(",")("build your own game", getting_started)(", or")("receive advice on engine choice", engine_choice)(". You can choose links by clicking on them or pressing the keybind next to them.")(r);
}

int main() {
	EM_ASM(emscripten_loaded = true;);
	o(R"(<h1 style="text-align:center;">Text Game Standard Engine</h1>This webpage was built in the engine.)")(suppress_history)("Explore this engine by clicking here!", main_menu)(r);
}
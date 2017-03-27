To get started:

0. take a test drive at https://interactivetext.github.io/
1. install <a href="https://kripken.github.io/emscripten-site/docs/getting_started/downloads.html">emscripten</a>, the compiler that does most of the work
2. download this repository
3. open a console in the project folder (on Windows, shift right-click when nothing is selected), and compile by pasting this command:
`emcc htmloutput.cpp sample.cpp --shell-file shell.html -o index.html -std=c++1z -s NO_EXIT_RUNTIME=1 -O3 --closure 1`
4. run index.html to test the compiled files. Don't use Internet Explorer for this step.
5. to start writing, modify sample.cpp, and compile again to see your changes.

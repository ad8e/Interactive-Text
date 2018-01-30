
See it in action at https://interactivetext.github.io/. To get started:

1. install <a href="https://kripken.github.io/emscripten-site/docs/getting_started/downloads.html">emscripten</a>, the compiler that does most of the work
2. <a href="https://github.com/ad8e/Interactive-Text/archive/master.zip">download this repository</a>
3. open a console in the repository folder (if on Windows, shift right-click when nothing is selected), and compile with this command:
`emcc htmloutput.cpp sample.cpp --shell-file shell.html -o index.html -std=c++1z -O3 --memory-init-file 0 -s 'EXTRA_EXPORTED_RUNTIME_METHODS=["UTF8ToString"]'`
4. run index.html to test the compiled files.
5. to start writing, modify sample.cpp, and compile again to see your changes.

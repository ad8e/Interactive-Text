#include "htmloutput.h"
/*
o("text") to add text.
o("link text", function_to_call_when_pressed) to add links
o(r) to flush your text to the screen.
chainable: o("Click")("this")("link", function)(r).
*/

void third_scene() {
	o("There's nothing here, because this is just a sample project.")(r);
}

void next_scene() {
	o("You spot a door, which will take you to the")("next next scene", third_scene)(".")(r);
}

int main() {
	o("This is the sample project.")("Move to the next scene.", next_scene)(r);
}
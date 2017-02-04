#include "htmloutput.h"
/*
o("text") to add text.
o("link text", function_to_call_when_pressed) to add links
o(r) to flush your text to the screen.
chainable: o("first")("second")("link", function)(r).
*/

void scary_goblins() {
	o("Oh no, you spot some scary goblins! This is the end!")(r);
}

void next_scene() {
	o("You spot a door, which will take you to the")("next next scene", scary_goblins)(".")(r);
}

int main() {
	o("This is the sample project.")("Move to the next scene!", next_scene)(r);
}
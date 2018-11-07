#include "o.h"
/*
Guide:
o("this text will be output")
o("this will be a link", function_to_call_when_pressed)
o(r) to display the page after it's fully constructed
chainable: o("Click")("this")("link", function)(r).
*/

void third_scene() {
	o("This is the end. You can also try replacing sample.cpp with intro.cpp in the compilation command if you want to see a bigger project being built.")(r);
}

void next_scene() {
	o("You spot a door, which will take you to the")("next next scene", third_scene)(".")(r);
}

int main() {
	o("This is the sample project.")("Move to the next scene.", next_scene)(r);
}
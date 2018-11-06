#include "o.h"
/*
Guide:
o("this text will be output")
o("this will be a link", function_to_call_when_pressed)
o(r) to display the page after it's fully constructed
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
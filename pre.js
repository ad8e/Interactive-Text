//async the css setting, otherwise it applies to first load
css('body', 'transition', 'background-color 0.5s, color 0.5s');

//keyboard shortcuts are enabled when the main module is loaded.
document.onkeypress = function(e) {
	if (e.repeat) return; //chrome seems to always give e.repeat=false. use this to diagnose: "document.onkeypress = function(e) {console.log(e);}"
	if (rebindingkeynumber !== -1) {
		//note: e.which is required, instead of e.charCode, because charCode doesn't support Enter
		switch (e.which) {
		case 0:
			alert('Only glyphs are supported, like letters, numbers, and punctuation. That excludes keys like arrows and Ctrl. This is because of javascript problems.');
			return;
		case 13:
			alert("Enter is a disallowed keybind, because you need Enter to select links when tabbing. Plus, Enter causes trouble with javascript's fromCharCode().");
			return;
		case 32:
			alert('Space is not recommended because it is invisible and will scroll your page down.');
			return;
		}
		//for (var i in keycoderaw) if (keycoderaw[i] == e.which) return; //this is annoying for intermediate state changes
		keycoderaw[rebindingkeynumber] = e.which;
		var key_text = String.fromCharCode(e.which);
		localStorage.setItem('keycode ' + rebindingkeynumber, key_text);
		css('#l' + rebindingkeynumber + '::after', 'content', '\'' + key_text + '\'');
		css('#c' + rebindingkeynumber + '::after', 'content', '\'[' + key_text + ']\'');
		css('#c' + rebindingkeynumber, 'font-weight', 'normal');
		rebindingkeynumber = -1;
		return;
	}
	for (var n in keycoderaw) {
		if ((e.which == keycoderaw[n]) && keyboard_shortcuts_enabled) {
			_i(-n - 1);
			return;
		}
	}
	if (e.which == 13) { //trigger onclick() with enter key. 13 = enter key.
		if (typeof document.activeElement.onmousedown == 'function') document.activeElement.onmousedown();
		return;
	}
};

function insert_history(text) {
	if (history_retention != 0) {
		var new_history = document.createElement('li');
		new_history.id = 'h' + (history_id++);
		new_history.innerHTML = UTF8ToString(text);
		history_container.insertBefore(new_history, history_container.firstChild);
	}
	clear_old_history();
}

function new_message(text) {
	main_body.innerHTML = UTF8ToString(text);
	keybound_links_nontabbable();
	if (history_above) main_body.scrollIntoView(true);
	else scrollTo(0, 0);
}
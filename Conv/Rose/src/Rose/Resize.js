var @@_Events = [];

function @@_Add(f) {
	@@_Events.push(f);
}

window.resize = function() {
	for(var i = 0; i < @@_Events.length; i++) {
		@@_Events[i]();
	}
}

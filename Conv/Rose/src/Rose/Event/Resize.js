var @@_Events = [];

window.onresize = function() {
	for(var i = 0; i < @@_Events.length; i++) {
		@@_Events[i]();
	}
}

function @@_Add(routine) {
	@@_Events.push(routine);
}

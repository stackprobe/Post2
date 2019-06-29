var @@_Queue = [];
var @@_Hashed = new Map();
var @@_UI = null;

window.onload = function() {
	@@_Main();
}

function @@_Main() {
	setInterval(@@_Interval, 100);

	Main(); // external
}

function @@_Interval() {
	if(@@_Chain_IsBusy()) {
		// noop
	}
	else if(1 <= @@_Queue.length) {
		var q = @@_Queue;

		@@_Queue = [];

		for(var i = 0; i < q.length; i++) {
			q[i]();
		}
	}
	else {
		var keys = Rose_GetMapKeys(@@_Hashed);

		if(1 <= keys.length) {
			var m = @@_Hashed;

			@@_Hashed = new Map();

			for(var i = 0; i < keys.length; i++) {
				m.get(keys[i])();
			}
		}
		else if(@@_UI != null) {
			@@_UI();
			@@_UI = null;
		}
	}
}

function @@_Add(routine) {
	@@_Queues.push(routine);
}

function @@_Set(key, routine) {
	@@_Hashed.set(key, routine);
}

function @@_SetUI(routine) {
	@@_UI = routine;
}

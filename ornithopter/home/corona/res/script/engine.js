// ENG_

// @encodeBgn

var ENG_Chain = [];
var ENG_Queues = [];
var ENG_Hashed = new Map();
var ENG_UI = null;

window.onload = function() {
	ENG_Main();
}

function ENG_Main() {
	setInterval(ENG_Interval, 100);
	Main(); // external
}

function ENG_Interval() {
	if(1 <= ENG_Queues.length) {
		var qs = [];

		for(var i = 0; i < ENG_Queues.length; i++) {
			var q = ENG_Queues[i];

			q.shift()();

			if(1 <= q.length) {
				qs.push(q);
			}
		}
		ENG_Queues = qs;
	}
	else {
		var keys = TKT_GetMapKeys(ENG_Hashed);

		if(1 <= keys.length) {
			var qs = new Map();

			for(var i = 0; i < keys.length; i++) {
				var q = ENG_Hashed.get(keys[i]);

				q.shift()();

				if(1 <= q.length) {
					qs.set(keys[i], q);
				}
			}
			ENG_Hashed = qs;
		}
		else if(ENG_UI != null) {
			ENG_UI();
			ENG_UI = null;
		}
	}
}

function ENG_FireChain() {
	if(1 <= ENG_Chain.length) {
		ENG_Chain.shift()(ENG_FireChain);
	}
}

function ENG_AddChain(f) {
	ENG_Chain.push(f);
	ENG_Chain.push(function(next) {
		next();
	});

	if(ENG_Chain.length == 2) {
		ENG_FireChain();
	}
}

function ENG_AddPostChain(f) {
	ENG_AddChain(function(next) {
		f();
		next();
	});
}

function ENG_Add(q) {
	ENG_Queues.push(q);
}

function ENG_Set(key, q) {
	ENG_Hashed.set(key, q);
}

function ENG_SetUI(ev) {
	ENG_UI = ev;
}

var ENG_ResizedEvents = [];

window.onresize = function() {
	for(var i = 0; i < ENG_ResizedEvents.length; i++) {
		ENG_ResizedEvents[i]();
	}
}

function ENG_AddResized(f) {
	ENG_ResizedEvents.push(f);
}

// @encodeEnd

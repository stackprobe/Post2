var @@_Count = 0;

function @@_Enter(f) {
	@@_Count++;
}

function @@_Leave() {
	@@_Count--;
}

var @@_Reaction;
var @@_Millis;

function @@_Wait(reaction) {
	@@_Reaction = reaction;
	@@_Millis = 0;

	@@_WaitLoop();
}

function @@_WaitLoop() {
	if(@@_Count == 0) {
		@@_Reaction();
	}
	else {
		if(@@_Millis < 100) {
			@@_Millis++;
		}
		setTimeout(@@_WaitLoop, @@_Millis);
	}
}

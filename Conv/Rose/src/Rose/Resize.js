var @@_Reactions = [];

function @@_Add(f) {
	@@_Reactions.push(f);
}

window.onresize = function() {
	for(var i = 0; i < @@_Reactions.length; i++) {
		@@_Reactions[i]();
	}
};

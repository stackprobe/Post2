var @@_Reactions = [];

function @@_Add(reaction) {
	@@_Reactions.push(reaction);
}

window.onresize = function() {
	for(var i = 0; i < @@_Reactions.length; i++) {
		@@_Reactions[i]();
	}
};

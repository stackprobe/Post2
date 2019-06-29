function @@_Set(scenario) {
	var next = null;

	for(var i = scenario.length - 1; 0 <= i; i--) {
		next = @@_Add(scenario[i], next);
	}
	@@^_Event_Add(next);
}

function @@_Add(routine, next) {
	if(next == null) {
		return routine;
	}
	else {
		return function() {
			routine();
			@@^_Event_Add(next);
		};
	}
}

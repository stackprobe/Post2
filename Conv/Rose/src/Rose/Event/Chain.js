var @@_Chain = [];

function @@_IsBusy() {
	return 1 <= @@_Chain.length;
}

function @@_Fire() {
	if(1 <= @@_Chain.length) {
		@@_Chain.shift()(@@_Fire);
	}
}

function @@_Add(routine) {
	@@_Chain.push(routine);
	@@_Chain.push(function(next) {
		next();
	});

	if(@@_Chain.length == 2) {
		@@_FireChain();
	}
}

function @@_Post(routine) {
	@@_Add(function(next) {
		routine();
		next();
	});
}

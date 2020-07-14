function @@_New(count, reaction) {
	return {
		Count    : count,
		Reaction : reaction,
	};
}

function @@_Kick(m) {
	m.Count--;

	if(m.Count == 0) {
		m.Reaction();
	}
}

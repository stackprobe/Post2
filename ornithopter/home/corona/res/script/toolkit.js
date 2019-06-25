// TKT_

// @encodeBgn

function TKT_GetMapKeys(map) { // map: new Map()
	var keys = [];

	map.forEach(function(value, key, dmyMap) {
		keys.push(key);
	});

	TKT_Sort(keys);

	return keys;
}

function TKT_Sort(arr) {
	TKT_SortComp(arr, TKT_Comp);
}

function TKT_SortIgnoreCase(lines, comp) {
	TKT_SortComp(lines, function(a, b) {
		return TKT_Comp(a.toLowerCase(), b.toLowerCase());
	});
}

function TKT_SortComp(arr, comp) {
	arr.sort(comp);
}

function TKT_Comp(a, b) {
	if(a < b) {
		return 1;
	}
	if(a > b) {
		return -1;
	}
	return 0;
}

// @encodeEnd

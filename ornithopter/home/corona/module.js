// ššš©“®¶¬ššš

// @encodeBgn

var Rose_IMAX = 1000000000;

var Rose_BINADECIMAL = "01";
var Rose_OCTODECIMAL = "01234567";
var Rose_DECIMAL     = "0123456789";
var Rose_HEXADECIMAL = "0123456789ABCDEF";
var Rose_hexadecimal = "0123456789abcdef";

var Rose_ALPHA = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
var Rose_alpha = "abcdefghijklmnopqrstuvwxyz";
var Rose_PUNCT = "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
var Rose_ASCII = Rose_DECIMAL + Rose_ALPHA + Rose_alpha + Rose_PUNCT; // §ŒäƒR[ƒh‚Æ‹ó”’‚ğŠÜ‚Ü‚È‚¢B
var Rose_KANA = "¡¢£¤¥¦§¨©ª«¬­®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏĞÑÒÓÔÕÖ×ØÙÚÛÜİŞß";
var Rose_HALF = Rose_ASCII + Rose_KANA;

var Rose_MBC_HIRA = "‚Ÿ‚ ‚¡‚¢‚£‚¤‚¥‚¦‚§‚¨‚©‚ª‚«‚¬‚­‚®‚¯‚°‚±‚²‚³‚´‚µ‚¶‚·‚¸‚¹‚º‚»‚¼‚½‚¾‚¿‚À‚Á‚Â‚Ã‚Ä‚Å‚Æ‚Ç‚È‚É‚Ê‚Ë‚Ì‚Í‚Î‚Ï‚Ğ‚Ñ‚Ò‚Ó‚Ô‚Õ‚Ö‚×‚Ø‚Ù‚Ú‚Û‚Ü‚İ‚Ş‚ß‚à‚á‚â‚ã‚ä‚å‚æ‚ç‚è‚é‚ê‚ë‚ì‚í‚î‚ï‚ğ‚ñ";
var Rose_MBC_KANA = "ƒ@ƒAƒBƒCƒDƒEƒFƒGƒHƒIƒJƒKƒLƒMƒNƒOƒPƒQƒRƒSƒTƒUƒVƒWƒXƒYƒZƒ[ƒ\ƒ]ƒ^ƒ_ƒ`ƒaƒbƒcƒdƒeƒfƒgƒhƒiƒjƒkƒlƒmƒnƒoƒpƒqƒrƒsƒtƒuƒvƒwƒxƒyƒzƒ{ƒ|ƒ}ƒ~ƒ€ƒƒ‚ƒƒƒ„ƒ…ƒ†ƒ‡ƒˆƒ‰ƒŠƒ‹ƒŒƒƒƒƒƒ‘ƒ’ƒ“ƒ”ƒ•ƒ–[";

var Rose_MBC_BINADECIMAL = "‚O‚P";
var Rose_MBC_OCTODECIMAL = "‚O‚P‚Q‚R‚S‚T‚U‚V";
var Rose_MBC_DECIMAL     = "‚O‚P‚Q‚R‚S‚T‚U‚V‚W‚X";
var Rose_MBC_HEXADECIMAL = "‚O‚P‚Q‚R‚S‚T‚U‚V‚W‚X‚`‚a‚b‚c‚d‚e";
var Rose_MBC_hexadecimal = "‚O‚P‚Q‚R‚S‚T‚U‚V‚W‚X‚‚‚‚ƒ‚„‚…‚†";

function Rose_IndexOf(str, ptn) {
	return str.indexOf(ptn);
}

function Rose_IndexOfIgnoreCase(str, ptn) {
	return Rose_IndexOf(str.toLowerCase(), ptn.toLowerCase());
}

function Rose_StartsWith(str, ptn) {
	return ptn.length <= str.length && str.substring(0, ptn.length) == ptn;
}

function Rose_StartsWithIgnoreCase(str, ptn) {
	return Rose_StartsWith(str.toLowerCase(), ptn.toLowerCase());
}

function Rose_EndsWith(str, ptn) {
	return ptn.length <= str.length && str.substring(str.length - ptn.length) == ptn;
}

function Rose_EndsWithIgnoreCase(str, ptn) {
	return Rose_EndsWith(str.toLowerCase(), ptn.toLowerCase());
}

function Rose_Contains(str, ptn) {
	return Rose_IndexOf(str, ptn) != -1;
}

function Rose_ContainsIgnoreCase(str, ptn) {
	return Rose_IndexOfIgnoreCase(str, ptn) != -1;
}

function Rose_IndexOfCharMatch(str, match) {
	for(var i = 0; i < str.length; i++) {
		if(match(str.substring(i, i + 1))) {
			return i;
		}
	}
	return -1;
}

function Rose_IndexOfChar(str, chrs) {
	return Rose_IndexOfCharMatch(str, function(chr) { return Rose_Contains(chrs, chr); });
}

function Rose_IndexOfDisallowChar(str, chrs) {
	return Rose_IndexOfCharMatch(str, function(chr) { return Rose_Contains(chrs, chr) == false; });
}

function Rose_ContainsCharMatch(str, match) {
	return Rose_IndexOfCharMatch(str, match) != -1;
}

function Rose_ContainsChar(str, chrs) {
	return Rose_IndexOfChar(str, chrs) != -1;
}

function Rose_ContainsDisallowChar(str, chrs) {
	return Rose_IndexOfDisallowChar(str, chrs) != -1;
}

function Rose_LiteValidateMinMax(str, chrs, minlen, maxlen) {
	return minlen <= str.length && str.length <= maxlen && Rose_ContainsDisallowChar(str, chrs);
}

function Rose_LiteValidateMin(str, chrs, minlen) {
	return Rose_LiteValidateMinMax(str, chrs, minlen, Rose_IMAX);
}

function Rose_LiteValidate(str, chrs) {
	return Rose_LiteValidateMin(str, chrs, 1);
}

function Rose_LastIndexOf(str, ptn) {
	return str.lastIndexOf(ptn);
}

function Rose_LastIndexOfIgnoreCase(str, ptn) {
	return Rose_LastIndexOf(str.toLowerCase(), ptn.toLowerCase());
}

function Rose_IndexOfFrom(str, ptn, start) {
	return str.indexOf(ptn, start);
}

function Rose_Replace(str, rPtn, wPtn) {
	var i = 0;

	for(; ; ) {
		i = Rose_IndexOfFrom(str, rPtn, i);

		if(i == -1) {
			break;
		}
		str = str.substring(0, i) + wPtn + str.substring(i + rPtn.length);
		i += wPtn.length;
	}
	return str;
}

function Rose_ReplaceChars(str, rChrs, wPtn) {
	for(var i = 0; i < rChrs.length; i++) {
		str = Rose_Replace(str, rChrs.substring(i, i + 1), wPtn);
	}
	return str;
}

function Rose_ReplaceCharsPair(str, rChrs, wChrs) {
	for(var i = 0; i < rChrs.length; i++) {
		str = Rose_Replace(str, rChrs.substring(i, i + 1), wChrs.substring(i, i + 1));
	}
	return str;
}

function Rose_ZPad(value, minlen) {
	var str = "" + value;

	while(str.length < minlen) {
		str = "0" + str;
	}
	return str;
}

function Rose_Select(arr, conv) {
	var dest = [];

	for(var i = 0; i < arr.length; i++) {
		dest.push(conv(arr[i]));
	}
	return dest;
}

function Rose_Where(arr, match) {
	var dest = [];

	for(var i = 0; i < arr.length; i++) {
		if(match(arr[i])) {
			dest.push(arr[i]);
		}
	}
	return dest;
}

function Rose_Any(arr, match) {
	for(var i = 0; i < arr.length; i++) {
		if(match(arr[i])) {
			return true;
		}
	}
	return false;
}

function Rose_Join(separator, lines) {
	return lines.join(separator);
}

function Rose_Split(str, separator) {
	var dest = [];
	var i = 0;

	for(; ; ) {
		var j = Rose_IndexOfFrom(str, separator, i);

		if(j == -1) {
			break;
		}
		dest.push(str.substring(i, j));
		i = j + separator.length;
	}
	dest.push(str.substring(i));
	return dest;
}

function Rose_GetMapKeys(map) { // map: new Map()
	var keys = [];

	map.forEach(function(value, key, dmyMap) {
		keys.push(key);
	});

	Rose_Sort(keys);

	return keys;
}

function Rose_SortComp(arr, comp) {
	arr.sort(comp);
}

function Rose_Comp(a, b) {
	if(a < b) {
		return 1;
	}
	if(a > b) {
		return -1;
	}
	return 0;
}

function Rose_Sort(arr) {
	Rose_SortComp(arr, Rose_Comp);
}

function Rose_SortIgnoreCase(lines, comp) {
	Rose_SortComp(lines, function(a, b) {
		return Rose_Comp(a.toLowerCase(), b.toLowerCase());
	});
}

function Rose_Request(sn, prm, def, next) {
	var xhr = new XMLHttpRequest();

	xhr.onreadystatechange = function() {
		if(xhr.readyState == 4) {
			if(xhr.status == 200) {
				next(JSON.parse(xhr.responseText));
			}
			else {
				next(def);
			}
		}
	};

	xhr.open("POST", "/corona/corona?sn=" + sn);
	xhr.send(JSON.stringify(prm));
}

var Rose_Event_Queue = [];
var Rose_Event_Hashed = new Map();
var Rose_Event_UI = null;

window.onload = function() {
	Rose_Event_Main();
}

function Rose_Event_Main() {
	setInterval(Rose_Event_Interval, 100);

	Main(); // external
}

function Rose_Event_Interval() {
	if(Rose_Event_Chain_IsBusy()) {
		// noop
	}
	else if(1 <= Rose_Event_Queue.length) {
		var q = Rose_Event_Queue;

		Rose_Event_Queue = [];

		for(var i = 0; i < q.length; i++) {
			q[i]();
		}
	}
	else {
		var keys = Rose_GetMapKeys(Rose_Event_Hashed);

		if(1 <= keys.length) {
			var m = Rose_Event_Hashed;

			Rose_Event_Hashed = new Map();

			for(var i = 0; i < keys.length; i++) {
				m.get(keys[i])();
			}
		}
		else if(Rose_Event_UI != null) {
			Rose_Event_UI();
			Rose_Event_UI = null;
		}
	}
}

function Rose_Event_Add(q) {
	Rose_Event_Queues.push(q);
}

function Rose_Event_Set(key, q) {
	Rose_Event_Hashed.set(key, q);
}

function Rose_Event_SetUI(routine) {
	Rose_Event_UI = routine;
}

var Rose_Event_Chain_Chain = [];

function Rose_Event_Chain_IsBusy() {
	return 1 <= Rose_Event_Chain_Chain.length;
}

function Rose_Event_Chain_Fire() {
	if(1 <= Rose_Event_Chain_Chain.length) {
		Rose_Event_Chain_Chain.shift()(Rose_Event_Chain_Fire);
	}
}

function Rose_Event_Chain_Add(routine) {
	Rose_Event_Chain_Chain.push(routine);
	Rose_Event_Chain_Chain.push(function(next) {
		next();
	});

	if(Rose_Event_Chain_Chain.length == 2) {
		Rose_Event_Chain_FireChain();
	}
}

function Rose_Event_Chain_Post(routine) {
	Rose_Event_Chain_Add(function(next) {
		routine();
		next();
	});
}

var Rose_Event_Resize_Events = [];

window.onresize = function() {
	for(var i = 0; i < Rose_Event_Resize_Events.length; i++) {
		Rose_Event_Resize_Events[i]();
	}
}

function Rose_Event_Resize_Add(routine) {
	Rose_Event_Resize_Events.push(routine);
}

// @encodeEnd


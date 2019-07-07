// ššš©“®¶¬ššš

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

function Rose_Request(sn, prm, def, reaction) {
	var xhr = new XMLHttpRequest();

	xhr.onreadystatechange = function() {
		if(xhr.readyState == 4) {
			if(xhr.status == 200) {
				reaction(JSON.parse(xhr.responseText));
			}
			else {
				reaction(def);
			}
		}
	};

	xhr.open("POST", "/corona/corona?sn=" + sn);
	xhr.send(JSON.stringify(prm));
}

function Rose_Forward(url, prm) {
	Rose_Request("save", prm, [ "" ], function(ret) {
		var q = ret[0];

		if(q == "") {
			alert("save error");
		}
		else {
			window.location.href = url + "?q=" + q;
		}
	});
}

var Rose_ERROR_PAGE_URL = "error.html";

function Rose_Forwarded(reaction) {
	var q = window.location.search.substring(3);

	Rose_Request("load", [ q ], null, function(ret) {
		if(ret == null) {
			window.location.href =Rose_ERROR_PAGE_URL;
		}
		else {
			reaction(ret);
		}
	});
}

window.onload = function() {
	Main(); // external
}

function Rose_Get(id) {
	return document.getElementById(id);
}

var Rose_Resize_Events = [];

function Rose_Resize_Add(f) {
	Rose_Resize_Events.push(f);
}

window.resize = function() {
	for(var i = 0; i < Rose_Resize_Events.length; i++) {
		Rose_Resize_Events[i]();
	}
}

// end


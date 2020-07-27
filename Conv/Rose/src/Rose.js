var @@_IMAX = 1000000000;

var @@_BINADECIMAL = "01";
var @@_OCTODECIMAL = "01234567";
var @@_DECIMAL     = "0123456789";
var @@_HEXADECIMAL = "0123456789ABCDEF";
var @@_hexadecimal = "0123456789abcdef";

var @@_ALPHA = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
var @@_alpha = "abcdefghijklmnopqrstuvwxyz";
var @@_PUNCT = "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
var @@_ASCII = @@_DECIMAL + @@_ALPHA + @@_alpha + @@_PUNCT; // §ŒäƒR[ƒh‚Æ‹ó”’‚ğŠÜ‚Ü‚È‚¢B
var @@_KANA = "¡¢£¤¥¦§¨©ª«¬­®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏĞÑÒÓÔÕÖ×ØÙÚÛÜİŞß";
var @@_HALF = @@_ASCII + @@_KANA;

var @@_MBC_HIRA = "‚Ÿ‚ ‚¡‚¢‚£‚¤‚¥‚¦‚§‚¨‚©‚ª‚«‚¬‚­‚®‚¯‚°‚±‚²‚³‚´‚µ‚¶‚·‚¸‚¹‚º‚»‚¼‚½‚¾‚¿‚À‚Á‚Â‚Ã‚Ä‚Å‚Æ‚Ç‚È‚É‚Ê‚Ë‚Ì‚Í‚Î‚Ï‚Ğ‚Ñ‚Ò‚Ó‚Ô‚Õ‚Ö‚×‚Ø‚Ù‚Ú‚Û‚Ü‚İ‚Ş‚ß‚à‚á‚â‚ã‚ä‚å‚æ‚ç‚è‚é‚ê‚ë‚ì‚í‚î‚ï‚ğ‚ñ";
var @@_MBC_KANA = "ƒ@ƒAƒBƒCƒDƒEƒFƒGƒHƒIƒJƒKƒLƒMƒNƒOƒPƒQƒRƒSƒTƒUƒVƒWƒXƒYƒZƒ[ƒ\ƒ]ƒ^ƒ_ƒ`ƒaƒbƒcƒdƒeƒfƒgƒhƒiƒjƒkƒlƒmƒnƒoƒpƒqƒrƒsƒtƒuƒvƒwƒxƒyƒzƒ{ƒ|ƒ}ƒ~ƒ€ƒƒ‚ƒƒƒ„ƒ…ƒ†ƒ‡ƒˆƒ‰ƒŠƒ‹ƒŒƒƒƒƒƒ‘ƒ’ƒ“ƒ”ƒ•ƒ–[";

var @@_MBC_BINADECIMAL = "‚O‚P";
var @@_MBC_OCTODECIMAL = "‚O‚P‚Q‚R‚S‚T‚U‚V";
var @@_MBC_DECIMAL     = "‚O‚P‚Q‚R‚S‚T‚U‚V‚W‚X";
var @@_MBC_HEXADECIMAL = "‚O‚P‚Q‚R‚S‚T‚U‚V‚W‚X‚`‚a‚b‚c‚d‚e";
var @@_MBC_hexadecimal = "‚O‚P‚Q‚R‚S‚T‚U‚V‚W‚X‚‚‚‚ƒ‚„‚…‚†";

function @@_IndexOf(str, ptn) {
	return str.indexOf(ptn);
}

function @@_IndexOfIgnoreCase(str, ptn) {
	return @@_IndexOf(str.toLowerCase(), ptn.toLowerCase());
}

function @@_StartsWith(str, ptn) {
	return ptn.length <= str.length && str.substring(0, ptn.length) == ptn;
}

function @@_StartsWithIgnoreCase(str, ptn) {
	return @@_StartsWith(str.toLowerCase(), ptn.toLowerCase());
}

function @@_EndsWith(str, ptn) {
	return ptn.length <= str.length && str.substring(str.length - ptn.length) == ptn;
}

function @@_EndsWithIgnoreCase(str, ptn) {
	return @@_EndsWith(str.toLowerCase(), ptn.toLowerCase());
}

function @@_Contains(str, ptn) {
	return @@_IndexOf(str, ptn) != -1;
}

function @@_ContainsIgnoreCase(str, ptn) {
	return @@_IndexOfIgnoreCase(str, ptn) != -1;
}

function @@_IndexOfCharMatch(str, match) {
	for(var i = 0; i < str.length; i++) {
		if(match(str.substring(i, i + 1))) {
			return i;
		}
	}
	return -1;
}

function @@_IndexOfChar(str, chrs) {
	return @@_IndexOfCharMatch(str, function(chr) { return @@_Contains(chrs, chr); });
}

function @@_IndexOfDisallowChar(str, chrs) {
	return @@_IndexOfCharMatch(str, function(chr) { return @@_Contains(chrs, chr) == false; });
}

function @@_ContainsCharMatch(str, match) {
	return @@_IndexOfCharMatch(str, match) != -1;
}

function @@_ContainsChar(str, chrs) {
	return @@_IndexOfChar(str, chrs) != -1;
}

function @@_ContainsDisallowChar(str, chrs) {
	return @@_IndexOfDisallowChar(str, chrs) != -1;
}

function @@_LiteValidateMinMax(str, chrs, minlen, maxlen) {
	return minlen <= str.length && str.length <= maxlen && @@_ContainsDisallowChar(str, chrs);
}

function @@_LiteValidateMin(str, chrs, minlen) {
	return @@_LiteValidateMinMax(str, chrs, minlen, @@_IMAX);
}

function @@_LiteValidate(str, chrs) {
	return @@_LiteValidateMin(str, chrs, 1);
}

function @@_LastIndexOf(str, ptn) {
	return str.lastIndexOf(ptn);
}

function @@_LastIndexOfIgnoreCase(str, ptn) {
	return @@_LastIndexOf(str.toLowerCase(), ptn.toLowerCase());
}

function @@_IndexOfFrom(str, ptn, start) {
	return str.indexOf(ptn, start);
}

function @@_Replace(str, rPtn, wPtn) {
	var i = 0;

	for(; ; ) {
		i = @@_IndexOfFrom(str, rPtn, i);

		if(i == -1) {
			break;
		}
		str = str.substring(0, i) + wPtn + str.substring(i + rPtn.length);
		i += wPtn.length;
	}
	return str;
}

function @@_ReplaceChars(str, rChrs, wPtn) {
	for(var i = 0; i < rChrs.length; i++) {
		str = @@_Replace(str, rChrs.substring(i, i + 1), wPtn);
	}
	return str;
}

function @@_ReplaceCharsPair(str, rChrs, wChrs) {
	for(var i = 0; i < rChrs.length; i++) {
		str = @@_Replace(str, rChrs.substring(i, i + 1), wChrs.substring(i, i + 1));
	}
	return str;
}

function @@_ZPad(value, minlen) {
	var str = "" + value;

	while(str.length < minlen) {
		str = "0" + str;
	}
	return str;
}

function @@_Select(arr, conv) {
	var dest = [];

	for(var i = 0; i < arr.length; i++) {
		dest.push(conv(arr[i]));
	}
	return dest;
}

function @@_Where(arr, match) {
	var dest = [];

	for(var i = 0; i < arr.length; i++) {
		if(match(arr[i])) {
			dest.push(arr[i]);
		}
	}
	return dest;
}

function @@_Any(arr, match) {
	for(var i = 0; i < arr.length; i++) {
		if(match(arr[i])) {
			return true;
		}
	}
	return false;
}

function @@_Join(separator, lines) {
	return lines.join(separator);
}

function @@_Split(str, separator) {
	var dest = [];
	var i = 0;

	for(; ; ) {
		var j = @@_IndexOfFrom(str, separator, i);

		if(j == -1) {
			break;
		}
		dest.push(str.substring(i, j));
		i = j + separator.length;
	}
	dest.push(str.substring(i));
	return dest;
}

function @@_GetMapKeys(map) { // map: new Map()
	var keys = [];

	map.forEach(function(value, key, dmyMap) {
		keys.push(key);
	});

	@@_Sort(keys);

	return keys;
}

function @@_SortComp(arr, comp) {
	arr.sort(comp);
}

function @@_Comp(a, b) {
	if(a < b) {
		return 1;
	}
	if(a > b) {
		return -1;
	}
	return 0;
}

function @@_Sort(arr) {
	@@_SortComp(arr, @@_Comp);
}

function @@_SortIgnoreCase(lines, comp) {
	@@_SortComp(lines, function(a, b) {
		return @@_Comp(a.toLowerCase(), b.toLowerCase());
	});
}

function @@_Request(sn, prm, def, reaction) {
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

	xhr.open("POST", "/coffee/coffee?sn=" + sn);
	xhr.send(JSON.stringify(prm));
}

function @@_Forward(url, prm) {
	@@_Request("save", prm, null, function(ret) {
		if(ret == null) {
			alert("save error");
		}
		else {
			window.location.href = url + "?q=" + ret;
		}
	});
}

function @@_Forwarded(reaction) {
	var q = window.location.search.substring(3);

	@@_Request("load", q, null, function(ret) {
		reaction(ret);

// old
/*
		if(ret == null) {
			alert("load error");
		}
		else {
			reaction(ret);
		}
*/

	});
}

window.onload = function() {
	Main(); // external
}

function @@_Get(id) {
	return document.getElementById(id);
}

function @@_Append(parentTag, tag) {
	parentTag.appendChild(tag);
}

function @@_Remove(tag) {
	tag.parentNode.removeChild(tag);
}

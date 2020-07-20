// ����������������

// ---- DD_Main �Ăяo���O�ɕύX��

var DD_W = 1920;
var DD_H = 1080;

// ----

var DD_GameIte;
var DD_Canvas;
var DD_CanvasBox;

function DD_Main(gameMain) {
	DD_GameIte = gameMain();

	DD_Canvas = document.createElement("canvas");
	DD_Canvas.style.position = "fixed";
	DD_Canvas.width  = DD_W;
	DD_Canvas.height = DD_H;

	DD_CanvasBox = document.createElement("div");
	DD_CanvasBox.style.position = "fixed";

	document.body.appendChild(DD_Canvas);
	document.body.appendChild(DD_CanvasBox);

	DD_Mouse_INIT(DD_Canvas);

	Rose_Resize_Add(function() {
		clearTimeout(DD_ResizedTimeoutId);
		DD_ResizedTimeoutId = setTimeout(DD_Resized, 100);
	});

	DD_Resized();
	DD_PostEachFrame();
	DD_Anime();
}

var DD_ResizedTimeoutId = 0;

function DD_Resized() {
	var sw = window.innerWidth;
	var sh = window.innerHeight;

	var w = sw;
	var h = Math.round((DD_H * sw) / DD_W);

	if(sh < h) {
		h = sh;
		w = Math.round((DD_W * sh) / DD_H);
	}
	var l = Math.round((sw - w) / 2);
	var t = Math.round((sh - h) / 2);

	DD_Canvas.style.left   = l + "px";
	DD_Canvas.style.top    = t + "px";
	DD_Canvas.style.width  = w + "px";
	DD_Canvas.style.height = h + "px";

	DD_CanvasBox.style.left   = l + "px";
	DD_CanvasBox.style.top    = t + "px";
	DD_CanvasBox.style.width  = w + "px";
	DD_CanvasBox.style.height = h + "px";
}

var DD_Time = 0;
var DD_Frame = 0;

function DD_Anime() {
	var currTime = new Date().getTime();

	DD_Time = Math.max(DD_Time, currTime - 100);
	DD_Time = Math.min(DD_Time, currTime + 100);

	if(DD_Time < currTime) {
		DD_GameIte.next();
		DD_Time += 16;
		DD_Frame++;
	}
	requestAnimationFrame(DD_Anime);
}

var DD_Ctx;

function DD_EachFrame() {
	// none

	// ----

	DD_PostEachFrame();
}

function DD_PostEachFrame() {
	DD_Ctx = DD_Canvas.getContext("2d");
}

function DD_Clear() {
	DD_Ctx.clearRect(0, 0, DD_W, DD_H) ;
}

function DD_Draw(image, x, y, a, r, z) {
	var w = image.naturalWidth;
	var h = image.naturalHeight;

	w *= z;
	h *= z;

	var l = x - w / 2;
	var t = y - h / 2;

	DD_Ctx.globalAlpha = a;

	DD_Ctx.translate(x, y);
	DD_Ctx.rotate(r);
	DD_Ctx.translate(-x, -y);

	DD_Ctx.drawImage(image, l, t, w, h);

	// restore
	DD_Ctx.translate(x, y);
	DD_Ctx.rotate(-r);
	DD_Ctx.translate(-x, -y);

	DD_Ctx.globalAlpha = 1.0; // restore
}

var Rose_IMAX = 1000000000;

var Rose_BINADECIMAL = "01";
var Rose_OCTODECIMAL = "01234567";
var Rose_DECIMAL     = "0123456789";
var Rose_HEXADECIMAL = "0123456789ABCDEF";
var Rose_hexadecimal = "0123456789abcdef";

var Rose_ALPHA = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
var Rose_alpha = "abcdefghijklmnopqrstuvwxyz";
var Rose_PUNCT = "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
var Rose_ASCII = Rose_DECIMAL + Rose_ALPHA + Rose_alpha + Rose_PUNCT; // ����R�[�h�Ƌ󔒂��܂܂Ȃ��B
var Rose_KANA = "���������������������������������������������������������������";
var Rose_HALF = Rose_ASCII + Rose_KANA;

var Rose_MBC_HIRA = "�����������������������������������������������������������������������ÂĂłƂǂȂɂʂ˂̂͂΂ςЂт҂ӂԂՂւׂ؂قڂۂ܂݂ނ߂��������������������";
var Rose_MBC_KANA = "�@�A�B�C�D�E�F�G�H�I�J�K�L�M�N�O�P�Q�R�S�T�U�V�W�X�Y�Z�[�\�]�^�_�`�a�b�c�d�e�f�g�h�i�j�k�l�m�n�o�p�q�r�s�t�u�v�w�x�y�z�{�|�}�~�����������������������������������������������[";

var Rose_MBC_BINADECIMAL = "�O�P";
var Rose_MBC_OCTODECIMAL = "�O�P�Q�R�S�T�U�V";
var Rose_MBC_DECIMAL     = "�O�P�Q�R�S�T�U�V�W�X";
var Rose_MBC_HEXADECIMAL = "�O�P�Q�R�S�T�U�V�W�X�`�a�b�c�d�e";
var Rose_MBC_hexadecimal = "�O�P�Q�R�S�T�U�V�W�X������������";

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
	Rose_Request("save", prm, null, function(ret) {
		if(ret == null) {
			alert("save error");
		}
		else {
			window.location.href = url + "?q=" + ret;
		}
	});
}

function Rose_Forwarded(reaction) {
	var q = window.location.search.substring(3);

	Rose_Request("load", q, null, function(ret) {
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

function Rose_Get(id) {
	return document.getElementById(id);
}

function Rose_Append(parentTag, tag) {
	parentTag.appendChild(tag);
}

function Rose_Remove(tag) {
	tag.parentNode.removeChild(tag);
}

function DD_LoadRes_Image(url) {
	var image = new Image();

	image.src = url;
	image.onload = Rose_Visitor_Leave;

	Rose_Visitor_Enter();

	return image;
}

function DD_LoadRes_Audio(url) {
	var audio = new Audio(url);

	audio.load();
	audio.addEventListener("canplaythrough", Rose_Visitor_Leave);

	Rose_Visitor_Enter();

	return audio;
}

var DD_Mouse_Down = false;
var DD_Mouse_X = 0;
var DD_Mouse_Y = 0;

function DD_Mouse_ScreenPosToCanvasPos() {
	var canvasRect = DD_Canvas.getBoundingClientRect();

	DD_Mouse_X -= canvasRect.left;
	DD_Mouse_Y -= canvasRect.top;
	DD_Mouse_X *= DD_W / canvasRect.width;
	DD_Mouse_Y *= DD_H / canvasRect.height;
}

function DD_Mouse_TouchStart(x, y) {
	DD_Mouse_Down = true;
	DD_Mouse_X = x;
	DD_Mouse_Y = y;

	DD_Mouse_ScreenPosToCanvasPos();
}

function DD_Mouse_TouchMove(x, y) {
	DD_Mouse_X = x;
	DD_Mouse_Y = y;

	DD_Mouse_ScreenPosToCanvasPos();
}

function DD_Mouse_TouchEnd(x, y) {
	DD_Mouse_Down = false;
	DD_Mouse_X = x;
	DD_Mouse_Y = y;

	DD_Mouse_ScreenPosToCanvasPos();
}

function DD_Mouse_GetEvTouch(touch) {
	return function(event) {
		touch(event.changedTouches[0].pageX, event.changedTouches[0].pageY);
	}
}

function DD_Mouse_GetEvMouse(touch) {
	return function(event) {
		touch(event.x, event.y);
	}
}

function DD_Mouse_INIT() {
	if(window.ontouchstart === null) {
		DD_CanvasBox.ontouchstart = DD_Mouse_GetEvTouch(DD_Mouse_TouchStart);
		DD_CanvasBox.ontouchmove  = DD_Mouse_GetEvTouch(DD_Mouse_TouchMove);
		DD_CanvasBox.ontouchend   = DD_Mouse_GetEvTouch(DD_Mouse_TouchEnd);
	}
	else {
		DD_CanvasBox.onmousedown  = DD_Mouse_GetEvMouse(DD_Mouse_TouchStart);
		DD_CanvasBox.onmousemove  = DD_Mouse_GetEvMouse(DD_Mouse_TouchMove);
		DD_CanvasBox.onmouseup    = DD_Mouse_GetEvMouse(DD_Mouse_TouchEnd);
		DD_CanvasBox.onmouseleave = DD_Mouse_GetEvMouse(DD_Mouse_TouchEnd);
	}
}

var Rose_Resize_Reactions = [];

function Rose_Resize_Add(reaction) {
	Rose_Resize_Reactions.push(reaction);
}

window.onresize = function() {
	for(var i = 0; i < Rose_Resize_Reactions.length; i++) {
		Rose_Resize_Reactions[i]();
	}
};

var Rose_Visitor_Count = 0;

function Rose_Visitor_Enter() {
	Rose_Visitor_Count++;
}

function Rose_Visitor_Leave() {
	Rose_Visitor_Count--;
}

var Rose_Visitor_Reaction;
var Rose_Visitor_Millis;

function Rose_Visitor_Wait(reaction) {
	Rose_Visitor_Reaction = reaction;
	Rose_Visitor_Millis = 0;

	Rose_Visitor_WaitLoop();
}

function Rose_Visitor_WaitLoop() {
	if(Rose_Visitor_Count == 0) {
		Rose_Visitor_Reaction();
	}
	else {
		if(Rose_Visitor_Millis < 100) {
			Rose_Visitor_Millis++;
		}
		setTimeout(Rose_Visitor_WaitLoop, Rose_Visitor_Millis);
	}
}

// �������������܂�


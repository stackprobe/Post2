// ---- @@_Main åƒÇ—èoÇµëOÇ…ïœçXâ¬

var @@_W = 1920;
var @@_H = 1080;

// ----

var @@_GameIte;
var @@_Canvas;
var @@_CanvasBox;

function @@_Main(gameMain) {
	@@_GameIte = gameMain();

	@@_Canvas = document.createElement("canvas");
	@@_Canvas.style.position = "fixed";
	@@_Canvas.width  = @@_W;
	@@_Canvas.height = @@_H;

	@@_CanvasBox = document.createElement("div");
	@@_CanvasBox.style.position = "fixed";

	document.body.appendChild(@@_Canvas);
	document.body.appendChild(@@_CanvasBox);

	@@_Mouse_INIT(@@_Canvas);

	Rose_Resize_Add(function() {
		clearTimeout(@@_ResizedTimeoutId);
		@@_ResizedTimeoutId = setTimeout(@@_Resized, 100);
	});

	@@_Resized();
	@@_PostEachFrame();
	@@_Anime();
}

var @@_ResizedTimeoutId = 0;

function @@_Resized() {
	var sw = window.innerWidth;
	var sh = window.innerHeight;

	var w = sw;
	var h = Math.round((@@_H * sw) / @@_W);

	if(sh < h) {
		h = sh;
		w = Math.round((@@_W * sh) / @@_H);
	}
	var l = Math.round((sw - w) / 2);
	var t = Math.round((sh - h) / 2);

	@@_Canvas.style.left   = l + "px";
	@@_Canvas.style.top    = t + "px";
	@@_Canvas.style.width  = w + "px";
	@@_Canvas.style.height = h + "px";

	@@_CanvasBox.style.left   = l + "px";
	@@_CanvasBox.style.top    = t + "px";
	@@_CanvasBox.style.width  = w + "px";
	@@_CanvasBox.style.height = h + "px";
}

var @@_Time = 0;
var @@_Frame = 0;

function @@_Anime() {
	var currTime = new Date().getTime();

	@@_Time = Math.max(@@_Time, currTime - 100);
	@@_Time = Math.min(@@_Time, currTime + 100);

	if(@@_Time < currTime) {
		@@_GameIte.next();
		@@_Time += 16;
		@@_Frame++;
	}
	requestAnimationFrame(@@_Anime);
}

var @@_Ctx;

function @@_EachFrame() {
	// none

	// ----

	@@_PostEachFrame();
}

function @@_PostEachFrame() {
	@@_Ctx = @@_Canvas.getContext("2d");
}

function @@_Clear() {
	@@_Ctx.clearRect(0, 0, @@_W, @@_H) ;
}

function @@_Draw(image, x, y, a, r, z) {
	var w = image.naturalWidth;
	var h = image.naturalHeight;

	w *= z;
	h *= z;

	var l = x - w / 2;
	var t = y - h / 2;

	@@_Ctx.globalAlpha = a;

	@@_Ctx.translate(x, y);
	@@_Ctx.rotate(r);
	@@_Ctx.translate(-x, -y);

	@@_Ctx.drawImage(image, l, t, w, h);

	// restore
	@@_Ctx.translate(x, y);
	@@_Ctx.rotate(-r);
	@@_Ctx.translate(-x, -y);

	@@_Ctx.globalAlpha = 1.0; // restore
}

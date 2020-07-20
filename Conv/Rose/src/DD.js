// ---- @@_Main �Ăяo���O�ɕύX��

var @@_W = 1920;
var @@_H = 1080;

// ----

var @@_GameLoop;
var @@_Canvas;

function @@_Main(gameLoop) {
	@@_GameLoop = gameLoop;

	@@_Canvas = document.createElement("canvas");
	@@_Canvas.style.position = "fixed";
	@@_Canvas.width  = @@_W;
	@@_Canvas.height = @@_H;

	document.body.appendChild(@@_Canvas);

	window.onresize = function() {
		clearTimeout(@@_ResizedTimeoutId);
		@@_ResizedTimeoutId = setTimeout(@@_Resized, 50);
	};

	@@_Resized();
	@@_Anime();
}

var @@_Time = 0;
var @@_Frame = 0;

function @@_Anime() {
	var currTime = new Date().getTime();

	@@_Time = Math.max(@@_Time, currTime - 100);
	@@_Time = Math.min(@@_Time, currTime + 100);

	if(@@_Time < currTime) {
		@@_Loop();
		@@_Time += 16;
		@@_Frame++;
	}
	requestAnimationFrame(@@_Anime);
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
}

var @@_Ctx;

function @@_Loop() {
	@@_Ctx = @@_Canvas.getContext("2d");

	@@_GameLoop();

	@@_Ctx = null;
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

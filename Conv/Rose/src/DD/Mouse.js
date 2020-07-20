var @@_Down = false;
var @@_X = 0;
var @@_Y = 0;

function @@_ScreenPosToCanvasPos() {
	var canvasRect = @@^_Canvas.getBoundingClientRect();

	@@_X -= canvasRect.left;
	@@_Y -= canvasRect.top;
	@@_X *= @@^_W / canvasRect.width;
	@@_Y *= @@^_H / canvasRect.height;
}

function @@_TouchStart(x, y) {
	@@_Down = true;
	@@_X = x;
	@@_Y = y;

	@@_ScreenPosToCanvasPos();
}

function @@_TouchMove(x, y) {
	@@_X = x;
	@@_Y = y;

	@@_ScreenPosToCanvasPos();
}

function @@_TouchEnd(x, y) {
	@@_Down = false;
	@@_X = x;
	@@_Y = y;

	@@_ScreenPosToCanvasPos();
}

function @@_GetEvTouch(touch) {
	return function(event) {
		touch(event.changedTouches[0].pageX, event.changedTouches[0].pageY);
	}
}

function @@_GetEvMouse(touch) {
	return function(event) {
		touch(event.x, event.y);
	}
}

function @@_INIT() {
	if(window.ontouchstart === null) {
		@@^_CanvasBox.ontouchstart = @@_GetEvTouch(@@_TouchStart);
		@@^_CanvasBox.ontouchmove  = @@_GetEvTouch(@@_TouchMove);
		@@^_CanvasBox.ontouchend   = @@_GetEvTouch(@@_TouchEnd);
	}
	else {
		@@^_CanvasBox.onmousedown  = @@_GetEvMouse(@@_TouchStart);
		@@^_CanvasBox.onmousemove  = @@_GetEvMouse(@@_TouchMove);
		@@^_CanvasBox.onmouseup    = @@_GetEvMouse(@@_TouchEnd);
		@@^_CanvasBox.onmouseleave = @@_GetEvMouse(@@_TouchEnd);
	}
}

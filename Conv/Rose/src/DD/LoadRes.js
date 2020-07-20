function @@_Image(url) {
	var image = new Image();

	image.src = url;
	image.onload = Rose_Visitor_Leave;

	Rose_Visitor_Enter();

	return image;
}

function @@_Audio(url) {
	var audio = new Audio(url);

	audio.load();

	return audio;
}

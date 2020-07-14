function Forwarded(reaction) {
	Rose_Forwarded(function(ret) {
		if(ret == null) {
			window.location.href = "error.html";
		}
		else {
			Rose_Request("salad/uploader&c=heartbeat", [ ret.SessionKey, null ], null, function(ret2) {
				if(ret2 == null) {
					window.location.href = "error.html";
				}
				else {
					reaction(ret);
				}
			});
		}
	});
}

var PageLoadingScreenTags;

function PageLoading() {
	var b = document.createElement("div");

	b.style.position = "fixed";
	b.style.top = "0px";
	b.style.left = "0px";
	b.style.width = "100%";
	b.style.height = "100%";
	b.style.backgroundColor = "#000000";
	b.style.opacity = "0.5";
	b.style.zIndex = "1";

	var p = document.createElement("div");

	p.style.position = "fixed";
	p.style.top = "30%";
	p.style.left = "10%";
	p.style.width = "80%";
	p.style.height = "40%";
	p.style.backgroundColor = "#ffffff";
	p.style.opacity = "1.0";
	p.style.zIndex = "2";

	p.style.font = "normal xx-large 'メイリオ'";

	p.innerText = "ページの読み込み中...";

	Rose_Append(document.body, b);
	Rose_Append(document.body, p);

	PageLoadingScreenTags = [ b, p ];
}

function PageLoaded() {
	Rose_Remove(PageLoadingScreenTags[0]);
	Rose_Remove(PageLoadingScreenTags[1]);
}

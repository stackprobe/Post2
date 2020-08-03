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

var PageLoadingTags = null;

function PageLoading() {
	if(PageLoadingTags == null) {
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
		p.style.top = "calc(50% - 64px)";
		p.style.left = "0px";
		p.style.width = "100%";
		p.style.height = "128px";
		p.style.backgroundColor = "#ffffff";
		p.style.opacity = "1.0";
		p.style.zIndex = "2";

		b.style.padding = "0px"; // todo: common.css
		p.style.padding = "0px"; // todo: common.css

		p.innerHTML = "<div style='padding: 0px; text-align: center'><img src='flat-preloaders/Preloader_2.gif'/></div>";

		Rose_Append(document.body, b);
		Rose_Append(document.body, p);

		PageLoadingTags = [ b, p ];
	}

	PageLoadingTags[0].style.visibility = "";
	PageLoadingTags[1].style.visibility = "";
}

function PageLoaded() {
	PageLoadingTags[0].style.visibility = "hidden";
	PageLoadingTags[1].style.visibility = "hidden";

// old
/*
	Rose_Remove(PageLoadingTags[0]);
	Rose_Remove(PageLoadingTags[1]);

	PageLoadingTags = null;
*/

}

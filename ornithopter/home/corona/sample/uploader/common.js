function Forwarded(reaction) {
	Rose_Forwarded(function(ret) {
		if(ret == null) {
			window.location.href = "error.html";
		}
		else {
			Rose_Request("sample/uploader&c=heartbeat", [ ret.SessionKey, null ], null, function(ret2) {
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

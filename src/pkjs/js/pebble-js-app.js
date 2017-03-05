Pebble.addEventListener("ready",
  function(e) {
    console.log("JavaScript app ready and running!");
	
  }
);

Pebble.addEventListener("showConfiguration",
  function(e) {
	Pebble.openURL("http://ec2.plzgeek.com/lgpebble/config.php#" + localStorage.tv_ip);
  }
);

Pebble.addEventListener("webviewclosed",
  function(e) {
    var configuration = JSON.parse(decodeURIComponent(e.response));
    console.log("Configuration window returned: ", JSON.stringify(configuration));
	localStorage.tv_ip = configuration.ipaddress;
	localStorage.tv_address = "http://" + configuration.ipaddress + ":8080";
	localStorage.pair_code = configuration.paircode;
	requestPairCode();
  }
);

Pebble.addEventListener("appmessage",
  function(e) {
	console.log(JSON.stringify(e.payload));
	
	if (e.payload.address !== undefined && localStorage.tv_address !== undefined)
	{
		//tv_address = "http://" + e.payload.address + ":8080";
		//pair_code = e.payload.pair_code;
		pair();
	}
	
	if (e.payload.command == "remote")
	{
		if (e.payload.data == "pair")
		{
			requestPairCode();
		}
		else
		{
			// Send remote code to TV
			var body = "<?xml version=\"1.0\" encoding=\"utf-8\"?><envelope><api type=\"command\"><name>HandleKeyInput</name><value>" + e.payload.data + "</value></api></envelope>";
			post("/udap/api/command", body);
		}
	}
	
	if (e.payload.command == "LogIn")
	{
		post("/api", JSON.stringify(""));
	}

  }
);

function post(uri, data)
{
	var req = new XMLHttpRequest();
	req.setRequestHeader("Content-type","text/xml; charset=utf-8");
	req.setRequestHeader("User-Agent","UDAP/2.0");
	
	req.open('POST', localStorage.tv_address + uri, true);
	
	console.log(localStorage.tv_address + uri);
	
	req.onload = function(e) {
		if (req.readyState == 4 && req.status == 200) {
			if(req.status == 200) {
				console.log(req.responseText);
			} else { 
				pair();
				console.log("Error - trying to force the pair"); 
			}
		}
	};
	req.send(data);
}

function put(uri, data)
{
	var req = new XMLHttpRequest();
	req.setRequestHeader("Content-type","text/xml; charset=utf-8");
	req.setRequestHeader("User-Agent","UDAP/2.0");
	req.open('PUT', localStorage.tv_address + uri, true);
	req.onload = function(e) {
		if (req.readyState == 4 && req.status == 200) {
			if(req.status == 200) {
				console.log(req.responseText);
			} else { console.log("Error"); }
		}
	};
	req.send(data);
}

function get(uri, data)
{
	var req = new XMLHttpRequest();
	req.open('GET', localStorage.tv_address + uri + data, true);
	req.onload = function(e) {
		if (req.readyState == 4 && req.status == 200) {
			if(req.status == 200) {
				console.log(req.responseText);
			} else { 
				if (uri != "/udap/api/pairing")
				{
					// Avoid pairing loop
					pair();
				}
				console.log("Error"); 
			}
		}
	};
	req.send(null);
}

function pair()
{
	var body = "<?xml version=\"1.0\" encoding=\"utf-8\"?><envelope><api type=\"pairing\"><name>hello</name><value>" + localStorage.pair_code + "</value><port>8080</port></api></envelope>";
	post("/udap/api/pairing", body);
}

function requestPairCode()
{
	var body = "<?xml version=\"1.0\" encoding=\"utf-8\"?><envelope><api type=\"pairing\"><name>showKey</name></api></envelope>";
	post("/udap/api/pairing", body);
	console.log(localStorage.tv_address);
}
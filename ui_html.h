const char ui_html[] PROGMEM = R"(
<head>
    <link rel="stylesheet" href="/styles.css">
</head>
<body>  
    <h3>Bytetender Demo</h3>
    <h2>By Nishil, Alex, and Mitchell</h2>

    <h1>Drink Selection</h1>



<table border="0" callpadding=6>
    <tr><td><button onclick='drink1()'>Grinch Punch</button></td></tr>
    <tr><td><button onclick='drink2()'>Shirley Temple</button></td></tr>
    <tr><td><button onclick='drink3()'>Sprite</button></td></tr>
    <tr><td><button onclick='drink4()'>Ginger Ale</button></td></tr>
</table>


<script>
    function id(s) { return document.getElementById(s); }
    function ajax(url, callback=null) {
	    let xhr=new XMLHttpRequest();
	    xhr.onreadystatechange = function() {
		    if(xhr.readyState==4 && xhr.status==200) {
			    if(callback) {
			        let data = JSON.parse(xhr.responseText);
			        callback(data);
			    }
		    }
	};
	xhr.open('GET', url, true);
	xhr.send();
}


function drink1() {
    ajax('set?type=drink&value=1');
}

function drink2() {
    ajax('set?type=drink&value=2');
}

function drink3() {
    ajax('set?type=drink&value=3');
}

function drink4() {
    ajax('set?type=drink&value=4');
}

function loadValues() {
    // load values from server if the bytetender is busy with a drink
}

setInterval(loadValues, 1000);

</script>
</body>

)";



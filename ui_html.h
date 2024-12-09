const char ui_html[] PROGMEM = R"(
<h3>ESP32 Cloud Demo (UI)</h3>
<table border=0 cellpadding=6>
<tr><td><button onclick='led_on()'>Turn LED On</button></td><td><button onclick='led_off()'>Turn LED Off</button></td></tr>
<tr><td><button onclick='play_tune()'>Play Tune</button></td><td><input type='text' value='1155665044332210' maxlength=24 id='tune'></td></tr>
<tr><td><button onclick='display_msg()'>Display Msg</button></td><td><input type='text' value='Hello World!' maxlength=24 id='msg'></td></tr>
<tr><td><button onclick='show_color()'>Show Color</button></td><td><input type='text' value='FF00FF' maxlength=6 id='color'></td></tr>
<tr><td>LED:</td><td><label id='lbl_led'>-</label></td></tr>
<tr><td>Light sensor:</td><td><label id='lbl_light'>-</label></td></tr>
<tr><td>Buttons:</td><td><label id='lbl_buttons'>-</label></td></tr>
<tr><td>Touch pin:</td><td><label id='lbl_touch'>-</label></td></tr>
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

function led_on() {
	ajax('set?type=led&value=1');
}

function led_off() {
	ajax('set?type=led&value=0');
}

function play_tune() {
	ajax('set?type=tune&value='+id('tune').value);
}

function display_msg() {
	ajax('set?type=msg&value='+id('msg').value);
}

function show_color() {
	ajax('set?type=color&value='+id('color').value);
}

function loadValues() {
	ajax('get', function(data) {
		id('lbl_led').innerHTML = data.led ? 'ON' : 'off';
		id('lbl_light').innerHTML = data.light;
		id('lbl_buttons').innerHTML = data.buttons;
		id('lbl_touch').innerHTML = data.touch;
	});
}

setInterval(loadValues,500);

</script>
)";

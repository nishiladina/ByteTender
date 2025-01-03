const char ui_html[] PROGMEM = R"(
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ByteTender Demo</title>
    <style>
        * {
            margin: 0;
            padding: 0;
        }

        body {
            font-family: Arial, sans-serif;
            background-color: #121212; /* Dark theme background */
            color: #ffffff; /* Light text for contrast */
            margin: 0;
            padding: 20px;
            text-align: center;
        }

        h3, h2, h1, table {
            color: #007BFF;
            margin: 0 auto;
            text-align: center;
        }

        /* Table Styling */
        table {
            margin: 0 auto;
            border-collapse: collapse;
            width: 100%; /* Responsive table */
        }

        td {
            padding: 10px;
        }

        /* Button Styling */
        button {
            background-color: #007BFF;
            color: white;
            padding: 10px 20px;
            border: none;
            border-radius: 5px;
            font-size: 16px;
            cursor: pointer;
            transition: background-color 0.3s ease;
        }

        button:hover {
            background-color: #0056b3;
        }

        button:active {
            background-color: #003f7f;
        }

        button:focus {
            outline: 3px solid #0056b3; /* Blue focus ring */
        }

        /* Title Styling */
        h3 {
            font-size: 24px;
            margin-bottom: 20px;
        }
    </style>
</head>
<body>
    <h3>ByteTender Demo</h3>
    <h2>By Nishil, Alex, and Mitchell</h2>

    <h1>Drink Selection</h1>

    <table border="0" cellpadding="6">
        <tr><td><button onclick='drink1()'>Grinch Punch</button></td></tr>
        <tr><td><button onclick='drink2()'>Shirley Temple</button></td></tr>
        <tr><td><button onclick='drink3()'>Sprite</button></td></tr>
        <tr><td><button onclick='drink4()'>Ginger Ale</button></td></tr>
        <tr><td><button onclick='drink5()'>Hawaiian Punch</button></td></tr>
        <tr><td>Status:</td><td><label id='lbl_status'>-</label></td></tr>
    </table>

    <script>
        function id(s) { return document.getElementById(s); }

        function ajax(url, callback=null) {
            let xhr = new XMLHttpRequest();
            xhr.onreadystatechange = function() {
                if (xhr.readyState == 4 && xhr.status == 200) {
                    if (callback) {
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

        function drink5() {
            ajax('set?type=drink&value=5');
        }

        function loadValues() {
            ajax('get', function(data) {
                id('lbl_status').innerHTML = data.busy ? 'busy' : 'not busy';
            });
        }

        setInterval(loadValues, 500);
    </script>
</body>
</html>
)";

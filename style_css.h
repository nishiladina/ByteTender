const char ui_css[] PROGMEM = R"(
body {
    font-family: Arial, sans-serif;
    background-color: #121212; /* Dark theme background */
    color: #ffffff; /* Light text for contrast */
    margin: 0;
    padding: 20px;
    text-align: center;
}

h3, h2, h1, table {
    margin: 0 auto;
    text-align: center;
}
/* Table Styling */
table {
    margin: 0 auto;
    border-collapse: collapse;
    width: 50%;
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

/* Title Styling */
h3 {
    font-size: 24px;
    margin-bottom: 20px;
}
)";
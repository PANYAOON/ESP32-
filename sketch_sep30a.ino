#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT11

#define LED1_PIN 27
#define LED2_PIN 26
#define SWITCH_PIN 25

DHT dht(DHTPIN, DHTTYPE);
WebServer server(80);

// ================= WIFI =================

const char* ssid = "FomosSnake";
const char* password = "4949494949";

// ================= LED STATUS =================

bool led1State = false;
bool led2State = false;


// ================= WEB PAGE =================

String webpage() {

  String html = R"rawliteral(

<!DOCTYPE html>
<html>

<head>

<meta name="viewport" content="width=device-width, initial-scale=1">

<title>ESP32 DHT11</title>

<style>

* {
  box-sizing: border-box;
}

body {
  margin: 0;
  padding: 15px;
  background: #000000;
  color: white;
  font-family: Arial, sans-serif;
}

h1 {
  text-align: center;
  font-size: 28px;
  margin: 10px 0 20px 0;
  color: white;
}

.box {
  background: #111111;
  border: 1px solid #333333;
  border-radius: 18px;
  padding: 18px;
  margin: 15px auto;
  max-width: 380px;
  text-align: center;
  box-shadow: 0 0 15px rgba(255,255,255,0.08);
}

h2 {
  margin-top: 0;
  margin-bottom: 15px;
  font-size: 24px;
}


/* ================= GAUGE ================= */

.gauge {
  width: 180px;
  height: 180px;
  border-radius: 50%;
  margin: auto;

  display: flex;
  justify-content: center;
  align-items: center;

  background:
    conic-gradient(
      #00aaff 0deg,
      #333333 0deg
    );
}

.gauge-inner {
  width: 130px;
  height: 130px;

  background: #111111;

  border-radius: 50%;

  display: flex;
  justify-content: center;
  align-items: center;

  flex-direction: column;
}

.value {
  font-size: 38px;
  font-weight: bold;
  color: white;
}

.unit {
  font-size: 20px;
  color: #bbbbbb;
}


/* ================= BUTTON ================= */

button {
  width: 110px;
  padding: 12px;
  margin: 5px;

  border: none;
  border-radius: 10px;

  font-size: 18px;
  font-weight: bold;

  cursor: pointer;
}

.on {
  background: #00c853;
  color: white;
}

.off {
  background: #d50000;
  color: white;
}


/* ================= STATUS ================= */

.status {
  margin-top: 12px;
  font-size: 20px;
  font-weight: bold;
}

</style>

</head>


<body>


<h1>ESP32 DHT11</h1>


<!-- ================= TEMPERATURE ================= -->

<div class="box">

<h2>Temperature</h2>

<div class="gauge" id="tempGauge">

  <div class="gauge-inner">

    <div class="value" id="tempValue">
      --
    </div>

    <div class="unit">
      C
    </div>

  </div>

</div>

</div>



<!-- ================= LED 1 ================= -->

<div class="box">

<h2>LED 1</h2>

<button class="on" onclick="led1On()">
ON
</button>

<button class="off" onclick="led1Off()">
OFF
</button>

<div class="status" id="led1Status">
LED1: OFF
</div>

</div>



<!-- ================= HUMIDITY ================= -->

<div class="box">

<h2>Humidity</h2>

<div class="gauge" id="humGauge">

  <div class="gauge-inner">

    <div class="value" id="humValue">
      --
    </div>

    <div class="unit">
      %
    </div>

  </div>

</div>

</div>



<!-- ================= LED 2 ================= -->

<div class="box">

<h2>LED 2</h2>

<button class="on" onclick="led2On()">
ON
</button>

<button class="off" onclick="led2Off()">
OFF
</button>

<div class="status" id="led2Status">
LED2: OFF
</div>

</div>



<script>


// ================= UPDATE DATA =================

function updateData() {

  fetch('/data')

  .then(response => response.json())

  .then(data => {

    // ================= TEMPERATURE =================

    let temp = Math.round(data.temperature);

    document.getElementById(
      "tempValue"
    ).innerHTML = temp;

    let tempAngle =
      (temp / 60) * 360;

    document.getElementById(
      "tempGauge"
    ).style.background =

      "conic-gradient(" +
      "#00aaff " +
      tempAngle +
      "deg, #333333 " +
      tempAngle +
      "deg)";


    // ================= HUMIDITY =================

    let hum = Math.round(data.humidity);

    document.getElementById(
      "humValue"
    ).innerHTML = hum;

    let humAngle =
      (hum / 100) * 360;

    document.getElementById(
      "humGauge"
    ).style.background =

      "conic-gradient(" +
      "#00e676 " +
      humAngle +
      "deg, #333333 " +
      humAngle +
      "deg)";


    // ================= LED 1 =================

    if (data.led1 == 1) {

      document.getElementById(
        "led1Status"
      ).innerHTML = "LED1: ON";

    } else {

      document.getElementById(
        "led1Status"
      ).innerHTML = "LED1: OFF";

    }


    // ================= LED 2 =================

    if (data.led2 == 1) {

      document.getElementById(
        "led2Status"
      ).innerHTML = "LED2: ON";

    } else {

      document.getElementById(
        "led2Status"
      ).innerHTML = "LED2: OFF";

    }

  });

}


// ================= LED 1 =================

function led1On() {

  fetch('/led1/on');

}

function led1Off() {

  fetch('/led1/off');

}


// ================= LED 2 =================

function led2On() {

  fetch('/led2/on');

}

function led2Off() {

  fetch('/led2/off');

}


// อัปเดตข้อมูลทุก 1 วินาที

setInterval(updateData, 1000);

updateData();

</script>


</body>

</html>

)rawliteral";

  return html;
}


// ================= ROOT =================

void handleRoot() {

  server.send(
    200,
    "text/html",
    webpage()
  );

}


// ================= LED 1 ON =================

void led1On() {

  led1State = true;

  digitalWrite(
    LED1_PIN,
    HIGH
  );

  server.send(
    200,
    "text/plain",
    "LED1 ON"
  );

}


// ================= LED 1 OFF =================

void led1Off() {

  led1State = false;

  digitalWrite(
    LED1_PIN,
    LOW
  );

  server.send(
    200,
    "text/plain",
    "LED1 OFF"
  );

}


// ================= LED 2 ON =================

void led2On() {

  led2State = true;

  digitalWrite(
    LED2_PIN,
    HIGH
  );

  server.send(
    200,
    "text/plain",
    "LED2 ON"
  );

}


// ================= LED 2 OFF =================

void led2Off() {

  led2State = false;

  digitalWrite(
    LED2_PIN,
    LOW
  );

  server.send(
    200,
    "text/plain",
    "LED2 OFF"
  );

}


// ================= SENSOR DATA =================

void sendData() {

  float temperature =
    dht.readTemperature();

  float humidity =
    dht.readHumidity();


  if (isnan(temperature)) {
    temperature = 0;
  }

  if (isnan(humidity)) {
    humidity = 0;
  }


  String data = "{";

  data += "\"temperature\":";
  data += temperature;

  data += ",";

  data += "\"humidity\":";
  data += humidity;

  data += ",";

  data += "\"led1\":";

  if (led1State) {
    data += "1";
  } else {
    data += "0";
  }

  data += ",";

  data += "\"led2\":";

  if (led2State) {
    data += "1";
  } else {
    data += "0";
  }

  data += "}";


  server.send(
    200,
    "application/json",
    data
  );

}


// ================= SETUP =================

void setup() {

  Serial.begin(115200);

  dht.begin();


  pinMode(
    LED1_PIN,
    OUTPUT
  );

  pinMode(
    LED2_PIN,
    OUTPUT
  );

  pinMode(
    SWITCH_PIN,
    INPUT_PULLUP
  );


  // เริ่มต้นให้ LED ดับ

  digitalWrite(
    LED1_PIN,
    LOW
  );

  digitalWrite(
    LED2_PIN,
    LOW
  );


  // ================= WIFI =================

  WiFi.mode(WIFI_AP);

  WiFi.softAP(
    ssid,
    password
  );


  Serial.println();

  Serial.println(
    "ESP32 WiFi Started"
  );

  Serial.print(
    "WiFi Name: "
  );

  Serial.println(
    ssid
  );

  Serial.print(
    "Password: "
  );

  Serial.println(
    password
  );

  Serial.print(
    "IP Address: "
  );

  Serial.println(
    WiFi.softAPIP()
  );


  // ================= WEB SERVER =================

  server.on(
    "/",
    handleRoot
  );

  server.on(
    "/led1/on",
    led1On
  );

  server.on(
    "/led1/off",
    led1Off
  );

  server.on(
    "/led2/on",
    led2On
  );

  server.on(
    "/led2/off",
    led2Off
  );

  server.on(
    "/data",
    sendData
  );


  server.begin();


  Serial.println(
    "Web Server Started"
  );

}


// ================= LOOP =================

void loop() {

  server.handleClient();

}
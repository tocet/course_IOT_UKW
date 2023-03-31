#include<ESP8266WiFi.h>
#include<ESPAsyncTCP.h>
#include<ESPAsyncWebServer.h>
#include<Wire.h>
#include<BH1750.h>
#define BAUDRATE 115200

const char* ssid = "ASUS";
const char* pass = "Doktorzec1";

AsyncWebServer server(80);

BH1750 light_sensor;
double lux = 0;

const char index_html[] PROGMEM = R"webpage(
<!DOCTYPE html><html>
<head>
  <meta name='viewport' content='width=device-width, initial-scale=1.0'>
  <title>ESP8266 Lux meter</title>
  <link href='https://fonts.googleapis.com/css?family=Open+Sans:300,400,600' rel='stylesheet'>
  <style>
   html { font-family: 'Open Sans', sans-serif; display: block; margin: 0px auto; text-align: center;color: #444444;}"
   body{margin: 0px;}"
   h1 {margin: 50px auto 30px;} 
   .side-by-side{display: table-cell;vertical-align: middle;position: relative;}
   .text{font-weight: 600;font-size: 19px;width: 200px;}
   .reading{font-weight: 200;font-size: 50px;padding-right: 25px;}
   .light.reading{color: #F29C1F;}
   .superscript{font-size: 17px;font-weight: 600;position: absolute;top: 10px;}
   .data{padding: 10px;}
   .container{display: table;margin: 0 auto;}
   .icon{width:65px}
   </style>
 <script>
  setInterval(updateLux,1000);
  function updateLux() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if(this.readyState == 4 && this.status == 200) { 
        document.getElementById("light_int").innerHTML = 
        this.responseText;} };
    xhttp.open("GET","light", true); 
    xhttp.send(); }
 </script>
</head>
<body>
  <h1>ESP8266 Lux meter</h1>
  <div class='container'>
  <div class='data light'>
  <div class='side-by-side icon'>
  <svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" version="1.1" width="60" height="60" viewBox="0 0 60 60" xml:space="preserve">
  <g transform="matrix(0.39 0 0 0.39 29 29)"><path style="stroke: rgb(238,238,40); 
  stroke-width: 8; stroke-dasharray: none; stroke-linecap: butt; stroke-dashoffset: 0; 
  stroke-linejoin: miter; stroke-miterlimit: 10; fill: rgb(238,238,40); fill-rule: evenodd; opacity: 1;"  
  transform=" translate(-59.83, -59.83)" d="m 0 59.82677 l 27.527777 -8.654488 l -19.512508 -21.258898 l 
  28.167 6.268881 l -6.268881 -28.167 l 21.258898 19.512508 l 8.654488 -27.527777 l 8.654491 27.527777 l 
  21.258896 -19.512508 l -6.2688828 28.167 l 28.167 -6.268881 l -19.512512 21.258898 l 27.527779 8.654488 l
  -27.527779 8.654491 l 19.512512 21.258896 l -28.167 -6.2688828 l 6.2688828 28.167 l -21.258896 -19.512512 l 
  -8.654491 27.527779 l -8.654488 -27.527779 l -21.258898 19.512512 l 6.268881 -28.167 l -28.167 6.2688828 l 19.512508 -21.258896 z" stroke-linecap="round" />
  </g></svg>
  </div>
  <div class='side-by-side text'>Light intensity</div>
  <div class='side-by-side reading'>
  <span id = "light_int"> 0 </span>
  <span class='superscript'>lx</span></div>
  </div>
</body></html>
)webpage";

void setup() {
Serial.begin(BAUDRATE);
WiFi.begin(ssid,pass);
Serial.print("Connecting to "); Serial.print(ssid); Serial.print("with password "); Serial.println(pass);
Serial.print("Start connection .");
while(WiFi.status()!=WL_CONNECTED) {
  delay(500);
  Serial.print("."); }
Serial.print("ESP8266 board IP "); Serial.print(WiFi.localIP());

Wire.begin();
light_sensor.begin();

//web page
server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

server.on("/light", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(200, "text/plain", String(lux));
  });

server.begin();
}

long p_millis = 0;
int i=0;
String message;
#define HR_MODE_DELAY 1000 // High Resolution Mode min.120[ms]

void loop() {
if(millis() - p_millis > HR_MODE_DELAY) {
  lux = light_sensor.readLightLevel();
  message = "Light intensity" + String(lux);
  Serial.println(message + "[lx]");
  p_millis = millis(); 
  }
}

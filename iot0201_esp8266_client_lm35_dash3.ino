// DASH 3.0.2
#define BAUDRATE 115200
#define TMP36pin A0

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPDash.h>

AsyncWebServer server(80);

ESPDash dashboard(&server);
Card title_card(&dashboard, GENERIC_CARD, "Title");
Card temperature_card(&dashboard, TEMPERATURE_CARD, "Temperature", "°C");
Card my_button(&dashboard, BUTTON_CARD, "Fire");

double temperature;
long p_millis=0;

const char* ssid = "ASUS";
const char* pass = "Doktorzec1";

void setup() {
 WiFi.begin(ssid,pass);
 Serial.begin(BAUDRATE);
 Serial.print("Connecting network ");
 while(WiFi.status()!=WL_CONNECTED) {
  Serial.print(".");
  delay(500); }  
 Serial.println(" done");
 Serial.print("IP adres: ");
 Serial.print(WiFi.localIP());
 
 server.begin();
 delay(500);
 String s = "Web Thermometer";
 title_card.update(s);
 dashboard.sendUpdates();

 my_button.attachCallback([&](bool value){ 
  Serial.println("Button: " + String((value)?"true":"false")); 
  digitalWrite(LED_BUILTIN,!value);
  my_button.update(value); 
  dashboard.sendUpdates() ; });
  
 pinMode(LED_BUILTIN,OUTPUT);
 }

#define MAIN_LOOP_DELAY 2000

void loop() {
if(millis()-p_millis>MAIN_LOOP_DELAY) {
 temperature = ((analogRead(TMP36pin)/1024.0)*3300-500)/10;
 Serial.print("Temperature[degC] = ");  Serial.println(temperature);
 temperature_card.update(String(temperature)); 
 dashboard.sendUpdates();
 p_millis = millis();}
}

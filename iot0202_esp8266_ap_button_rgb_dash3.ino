// DASH 3.0.2
#define BAUDRATE 115200
#define BUTTON 12         // D6
#define LIGHT_OUTPUT 2    // D4

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPDash.h>

AsyncWebServer server(80);

ESPDash dashboard(&server);

Card my_button(&dashboard, BUTTON_CARD, "Light!");
Card my_dimmer(&dashboard, SLIDER_CARD, "Dimmer_blue", "", 0, 1023);
Card my_dimmerR(&dashboard, SLIDER_CARD, "Dimmer_red", "", 0, 1023);

long p_millis=0;

// SSID
const char* ssid = "Light_switch_net";
// Password/Empty Password=Open AP   
const char* password = "";           

bool switch_state = false;
int light_intensity = 0;
int light_intensityR = 0;

void setup() {
 Serial.begin(BAUDRATE);
 WiFi.mode(WIFI_AP);
 WiFi.softAP(ssid, password);
 delay(1000);  
 Serial.print("\nIP Address: ");   Serial.println(WiFi.softAPIP());    
 
 server.begin();
 delay(500);

 my_button.attachCallback([&](bool value){
  Serial.println("Button: " + String((value)?"true":"false"));
  switch_state = value;
  my_button.update(value);
  dashboard.sendUpdates(); });

 my_dimmer.attachCallback([&](int value){
  Serial.println("Light intensity: "+String(value));
  light_intensity = value;
  my_dimmer.update(value);
  dashboard.sendUpdates(); });

 my_dimmerR.attachCallback([&](int value){
  Serial.println("Light intensity: "+String(value));
  light_intensityR = value;
  my_dimmerR.update(value);
  dashboard.sendUpdates(); });

  attachInterrupt(digitalPinToInterrupt(BUTTON),changeLight,RISING);
}

#define MAIN_LOOP_DELAY 100

void loop() {
  if(millis() - p_millis > MAIN_LOOP_DELAY)
   if(switch_state) { 
    analogWrite(LIGHT_OUTPUT,light_intensity);
    analogWrite(LIGHT_OUTPUT,light_intensityR); 
   } else { 
    analogWrite(LIGHT_OUTPUT,0);
    analogWrite(LIGHT_OUTPUT,0);
   };
}

ICACHE_RAM_ATTR void changeLight() {
  switch_state = !switch_state;
  Serial.println(switch_state);
  my_button.update(switch_state);
  dashboard.sendUpdates();
}

#include<ESP8266WiFi.h>
#define BAUDRATE 115200

const char* ssid = "ASUS";
const char* pass = "Doktorzec1";

void setup() {
  Serial.begin(BAUDRATE);
  WiFi.begin(ssid,pass);
  Serial.print("Connecting .");
  while(WiFi.status()!=WL_CONNECTED) {
    Serial.print(".");
    delay(250);
  }
  Serial.print("\nIP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
}

#include<ESP8266WiFi.h>
#include<ESP8266HTTPClient.h>
#include<ArduinoJson.h>

#define BAUDRATE 115200

const char* ssid = "ASUS";
const char* pass = "Doktorzec1";

void setup() {
 Serial.begin(BAUDRATE);
 WiFi.begin(ssid, pass);
 Serial.print("Connecting to WiFi ");
 while (WiFi.status() != WL_CONNECTED) {
  delay(250);  Serial.print("."); }
 Serial.println("\nConnected to the WiFi network ");
 Serial.print("IP ");  Serial.println(WiFi.localIP());
}

WiFiClient client;
HTTPClient http;
long p_millis = 0;
#define DELAY 5000

void loop() {
if(millis() - p_millis > DELAY) {
 http.useHTTP10(true);
 http.begin(client,"http://time.jsontest.com");
 int httpResponseCode = http.GET();
 Serial.print("Code: ");
 Serial.println(httpResponseCode);
 
 if(httpResponseCode > 0) {
  StaticJsonDocument<0> filter;
  filter.set(true);
  StaticJsonDocument<128> doc;
  DeserializationError error = deserializeJson(doc, http.getStream(), DeserializationOption::Filter(filter));
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  const char* date = doc["date"]; // "03-24-2023"
  Serial.print("date: "); Serial.println(date);
  long long milliseconds_since_epoch = doc["milliseconds_since_epoch"]; // 1679696659859
  const char* time = doc["time"]; // "10:24:19 PM"
  Serial.print("time: "); Serial.println(time);
 }
 http.end();
 p_millis = millis();
 }
}

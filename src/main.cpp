#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

int LED_BUILTIN = 2;
const char* ssid = "YOUR_SSID_HERE";
const char* password = "YOUR_PASSWORD_HERE";
String baseUrl = "YOUR_BASEURL_HERE";
HTTPClient httpClient;
int BUTTON_PIN = 23;
bool sendTaken = false;

void IRAM_ATTR isr() {
  sendTaken = true;
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON_PIN,INPUT_PULLUP);
  attachInterrupt(BUTTON_PIN,isr,FALLING);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  WiFi.begin(ssid,password);
}

void loop() {
  if(!WiFi.isConnected()) {
    WiFi.begin(ssid,password);
  }
  if(sendTaken) {

    sendTaken = false;
    Serial.println("Taken!");
    httpClient.begin(baseUrl + "taken");
    httpClient.GET();
    digitalWrite(LED_BUILTIN,LOW);

  } else {

    httpClient.begin(baseUrl + "led");
    int httpCode = httpClient.GET();
    // Serial.println(httpCode);

    if(httpCode > 0) {
      String turnLedOn = httpClient.getString();
      if(turnLedOn == "True") {
        digitalWrite(LED_BUILTIN,HIGH);
      }
      else if (turnLedOn == "False") {
        digitalWrite(LED_BUILTIN,LOW);
      }
    }

  }
  delay(10 * 1000);
}
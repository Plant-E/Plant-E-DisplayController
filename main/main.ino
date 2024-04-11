#include <ArduinoJson.h>
#define LED_BUILTIN 2

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  listenToCommands();
  
}

void listenToCommands(){
  if(Serial.available() <= 0){ return; }

  String data = Serial.readString();
  blinkLED();

  StaticJsonDocument<200> json;
  DeserializationError error = deserializeJson(json, data);

  if (error) {
    Serial.println("FAIL");
    return;
  }

  String command = json["command"];
  if(command == "INIT"){
    init();
  }

}

void init(){
  Serial.println("OK");
}

void blinkLED() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(200);
  digitalWrite(LED_BUILTIN, LOW);
  delay(200);
}

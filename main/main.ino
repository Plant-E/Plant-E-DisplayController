  #include <Wire.h>
  #include <Adafruit_GFX.h>
  #include <Adafruit_LEDBackpack.h>
  #include <ArduinoJson.h>

  #define LED_BUILTIN 2

  //Blink
  unsigned long previousMillis = 0;
  const long interval = 200;
  bool isBlinking = false;

  //Matrix
  Adafruit_8x8matrix matrix = Adafruit_8x8matrix();

  void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    matrix.begin(0x70);
    matrix.setBrightness(15);

    Serial.begin(115200);
    Serial.setTimeout(50);
  }

  void loop() {
    listenToCommands();
    handleBlink();
  }

  void listenToCommands(){
    if(Serial.available() <= 0){ return; }

    String data = Serial.readString();

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
    else if(command == "VISUALIZE"){
      visualize(json["image"]);
    }

    Serial.println("OK");
    blink();
  }

  void init(){
    Serial.println("OK");
  }
  void visualize(String image){
    Serial.println(image);

    // matrix.clear();
    // matrix.drawPixel(4, 4, LED_ON);
    // matrix.drawPixel(5, 5, LED_ON);
    // matrix.writeDisplay();
  }

  void blink() {
    isBlinking = true;
    previousMillis = millis();
    digitalWrite(LED_BUILTIN, HIGH);
  }

  void handleBlink() {
    if (isBlinking) {
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis >= interval) {
        digitalWrite(LED_BUILTIN, LOW);
        isBlinking = false;
      }
    }
  }
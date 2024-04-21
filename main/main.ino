    #include <Wire.h>
    #include <Adafruit_GFX.h>
    #include <Adafruit_LEDBackpack.h>
    #include <ArduinoJson.h>

    #define LED_BUILTIN 2

    //Blink
    unsigned long previousMillis = 0;
    const long interval = 200;
    bool isBlinking = false;

    //Display
    int display_height = 8;
    int display_width = 8;

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

    blink();
  }

  void init(){
    Serial.println("OK");
  }

  void visualize(JsonVariant imageJson) {
    matrix.clear();

    String response = "";

    for(int y = 0; y < display_height; y++) {
        for(int x = 0; x < display_width; x++) {
            int index = (y * display_width + x) * 4;

            int red = imageJson[String(index)].as<int>();
            int green = imageJson[String(index + 1)].as<int>();
            int blue = imageJson[String(index + 2)].as<int>();
            int alpha = imageJson[String(index + 3)].as<int>();

            response += "(" + String(red) + "," + String(green) + "," + String(blue) + ")";

            // Assuming LED_ON and LED_OFF are properly defined elsewhere in your code
            if(red == 255 && green == 255 && blue == 255 && alpha == 255){
                matrix.drawPixel(x, y, LED_ON);
            } else {
                matrix.drawPixel(x, y, LED_OFF);
            }
        }
    }

    matrix.writeDisplay();
    Serial.println(response);
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
#include <Wire.h>
#include <SX1503.h>

#define SX1503_IRQ_PIN    14
#define SX1503_RESET_PIN  15
#define SDA_PIN           18
#define SCL_PIN           19

SX1503 dev(SX1503_IRQ_PIN, SX1503_RESET_PIN);


void setup() {
  Serial.begin(115200);
  Wire.setSDA(SDA_PIN);
  Wire.setSCL(SCL_PIN);
  Wire.begin();
  dev.init();
}


void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    switch (c) {
      case 'x':
        if (DIGITALPOT_ERROR::NO_ERROR == dev.init()) {
          Serial.println("Device initialized.");
        }
        break;
      case 'i':
        dev.printDebug();
        break;
    }
  }
}

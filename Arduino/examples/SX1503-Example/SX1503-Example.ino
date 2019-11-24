#include <Wire.h>
#include <SX1503.h>

/*******************************************************************************
* Pin definitions and hardware constants.
*******************************************************************************/
#define LED0_PIN          13
#define SX1503_IRQ_PIN    14
#define SX1503_RESET_PIN  15
#define SDA_PIN           18
#define SCL_PIN           19

/*******************************************************************************
* Globals
*******************************************************************************/
SX1503 dev(SX1503_IRQ_PIN, SX1503_RESET_PIN);


void sample_callback_0(uint8_t pin, uint8_t level) {
  Serial.print("sample_callback_0(): Pin ");
  Serial.print(pin, DEC);
  Serial.print(" is now ");
  Serial.println((0 != level) ? "HIGH" : "LOW");
}


int8_t toggleGPIO(uint8_t pin) {
  int8_t ret = dev.digitalWrite(pin, (0 == dev.digitalRead(pin)) ? HIGH : LOW);
  if (0 == ret) {
    Serial.print("GPIO ");
    Serial.print(pin, DEC);
    Serial.print(": ");
    Serial.println((0 == dev.digitalRead(pin)) ? "LOW" : "HIGH");
  }
  else {
    Serial.print("digitalWrite() returns ");
    Serial.println(ret, DEC);
  }
  return ret;
}


int8_t init_sx1503() {
  int8_t ret = dev.gpioMode(0, OUTPUT);
  if (0 == ret) {  ret = dev.gpioMode(1, OUTPUT);             }
  if (0 == ret) {  ret = dev.gpioMode(2, OUTPUT_PULLUP);      }
  if (0 == ret) {  ret = dev.gpioMode(3, OUTPUT_PULLDOWN);    }
  if (0 == ret) {  ret = dev.gpioMode(8, INPUT);              }
  if (0 == ret) {  ret = dev.gpioMode(9, INPUT);              }
  if (0 == ret) {  ret = dev.gpioMode(10, INPUT_PULLUP);      }
  if (0 == ret) {  ret = dev.gpioMode(11, INPUT_PULLDOWN);    }
  if (0 == ret) {  ret = dev.attachInterrupt(9,  sample_callback_0, CHANGE);   }
  if (0 == ret) {  ret = dev.attachInterrupt(10, sample_callback_0, FALLING);  }
  if (0 == ret) {  ret = dev.attachInterrupt(11, sample_callback_0, RISING);   }
  return ret;
}


/*******************************************************************************
* Setup function
*******************************************************************************/
void setup() {
  Serial.begin(115200);
  Wire.setSDA(SDA_PIN);
  Wire.setSCL(SCL_PIN);
  Wire.begin();
  if (0 == dev.init()) {
    init_sx1503();
  }
}


/*******************************************************************************
* Main loop
*******************************************************************************/
void loop() {
  int8_t ret = 0;
  digitalWrite(LED0_PIN, LOW);
  if (Serial.available()) {
    char c = Serial.read();
    switch (c) {
      case 'I':
        ret = dev.init();
        Serial.print("init() returns ");
        Serial.println(ret, DEC);
        break;
      case 'R':
        ret = dev.reset();
        Serial.print("reset() returns ");
        Serial.println(ret, DEC);
        break;
      case 'x':
        ret = dev.refresh();
        Serial.print("refresh() returns ");
        Serial.println(ret, DEC);
        break;

      case '0':  // Toggle GPIO
        ret = toggleGPIO(0);
        break;
      case '1':  // Toggle GPIO
        ret = toggleGPIO(1);
        break;
      case '2':  // Toggle GPIO
        ret = toggleGPIO(2);
        break;
      case '3':  // Toggle GPIO
        ret = toggleGPIO(3);
        break;

      case '8':  // Read GPIO
        Serial.print("GPIO 8: ");
        Serial.println((0 == dev.digitalRead(8)) ? "LOW" : "HIGH");
        break;

      case 'X':  // Reconfigures the SX1503 as this sketch uses it.
        ret = init_sx1503();
        Serial.print("init_sx1503() returns ");
        Serial.println(ret, DEC);
        break;

      case 'i':
        dev.printDebug();
        break;

      case 'r':
        dev.printRegs();
        break;
    }
  }
  else if (dev.isrFired()) {
    ret = dev.poll();
    if (0 != ret) {
      Serial.print("poll() returns error code ");
      Serial.println(ret, DEC);
    }
  }
  else {
    digitalWrite(LED0_PIN, HIGH);
  }
}

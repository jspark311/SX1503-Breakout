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
SX1503 sx1503(SX1503_IRQ_PIN, SX1503_RESET_PIN);


/*******************************************************************************
* Functions to output things to the console
*******************************************************************************/

void printHelp() {
  Serial.print("\nSX1503 Example\n------------------------------------\n");
  Serial.print("?     This output\n");
  Serial.print("i     SX1503 info\n");
  Serial.print("x     Refresh register shadows\n");
  Serial.print("c     Configure SX1503 for sketch\n");
  Serial.print("I     Reinitialize\n");
  Serial.print("R     Reset\n");
  Serial.print("S     Serialize\n");
  Serial.print("r     Print register shadows.\n");
  Serial.print("u     Open route between selected column and row.\n");
  Serial.print("0-3   Toggle output pin <X>.\n");
  Serial.print("4-7   Read input pin <X>.\n");
}


/*******************************************************************************
* Callbacks
* Meant to look superficially like Arduino's attachInterrupt(). But the fxn
*   signature is a bit more versatile, and the execution of callbacks does NOT
*   happen on the interrupt stack. So you can delay() and perform I/O here if
*   desired. Ultimately, our stack will trace back up to wherever the sketch
*   invoked sx1503.poll().
*******************************************************************************/

/*
* Because the pin and its state are presented as arguments to this call, all the
*   logic from all the pins can be handled with a single callback if desired.
*/
void sample_callback_0(uint8_t pin, uint8_t level) {
  Serial.print("sample_callback_0(): Pin ");
  switch (pin) {
    case 9:   // Logic for pin 9.
    Serial.print(9, DEC);
      break;
    case 10:   // Logic for pin 10.
      Serial.print(10, DEC);
      break;
    default:   // Etc...
      break;
  }
  Serial.print(" is now ");
  Serial.println((0 != level) ? "HIGH" : "LOW");
}


/*
* Second callback because some people think it reads more cleanly.
*/
void sample_callback_1(uint8_t pin, uint8_t level) {
  Serial.print("sample_callback_1(): Pin ");
  Serial.print(pin, DEC);
  Serial.print(" is now ");
  Serial.println((0 != level) ? "HIGH" : "LOW");
}


/*******************************************************************************
* Demonstration fxns
*******************************************************************************/
/*
* Despite being outputs, we can digitalRead() on any pin and expect the correct
*   answer.
* digitalRead() will not result in I/O. It will be refreshed upon calling
*   sx1503.poll(). It will always return at a rate that is both constant, and
*   close to the latency you would expect from a platform GPIO read.
* digitalWrite(), however, will block while an i2c bus operation is running.
*/
int8_t toggleGPIO(uint8_t pin) {
  int8_t ret = sx1503.digitalWrite(pin, (0 == sx1503.digitalRead(pin)) ? HIGH : LOW);
  if (0 == ret) {
    Serial.print("GPIO ");
    Serial.print(pin, DEC);
    Serial.print(": ");
    Serial.println((0 == sx1503.digitalRead(pin)) ? "LOW" : "HIGH");
  }
  else {
    Serial.print("digitalWrite() returns ");
    Serial.println(ret, DEC);
  }
  return ret;
}


/*
* The SX1503 should be waiting for us to conf it.
*/
int8_t init_sx1503() {
  int8_t ret = sx1503.gpioMode(0, OUTPUT);
  if (0 == ret) {  ret = sx1503.gpioMode(1, OUTPUT);             }
  if (0 == ret) {  ret = sx1503.gpioMode(2, OUTPUT_PULLUP);      }
  if (0 == ret) {  ret = sx1503.gpioMode(3, OUTPUT_PULLDOWN);    }
  if (0 == ret) {  ret = sx1503.gpioMode(8, INPUT);              }
  if (0 == ret) {  ret = sx1503.gpioMode(9, INPUT);              }
  if (0 == ret) {  ret = sx1503.gpioMode(10, INPUT_PULLUP);      }
  if (0 == ret) {  ret = sx1503.gpioMode(11, INPUT_PULLDOWN);    }
  if (0 == ret) {  ret = sx1503.attachInterrupt(9,  sample_callback_0, CHANGE);   }
  if (0 == ret) {  ret = sx1503.attachInterrupt(10, sample_callback_0, FALLING);  }
  if (0 == ret) {  ret = sx1503.attachInterrupt(11, sample_callback_1, RISING);   }
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

  // Calling preserveOnDestroy(true) ahead of init() will prevent the class from
  //   resetting the switch on either init() or class teardown.
  // The SX1503 does not have non-volatile storage, so if the state is to be
  //   rebuilt after a power loss or reset(), the state will need to be
  //   serialized and initialized later with the resuling buffer.
  // See the advanced usage example for more details.
  // sx1503.preserveOnDestroy(true);
  if (0 == sx1503.init(&Wire)) {
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
      case '?': printHelp();  break;

      case 'I':
        ret = sx1503.init();
        Serial.print("init() returns ");
        Serial.println(ret, DEC);
        break;
      case 'R':
        ret = sx1503.reset();
        Serial.print("reset() returns ");
        Serial.println(ret, DEC);
        break;
      case 'x':
        ret = sx1503.refresh();
        Serial.print("refresh() returns ");
        Serial.println(ret, DEC);
        break;

      case '0':  // Toggle GPIO
      case '1':  // Toggle GPIO
      case '2':  // Toggle GPIO
      case '3':  // Toggle GPIO
        ret = toggleGPIO(c - 0x30);
        break;

      case '4':  // Read GPIO
      case '5':  // Read GPIO
      case '6':  // Read GPIO
      case '7':  // Read GPIO
        Serial.print("GPIO ");
        Serial.print((c - 0x30), DEC);
        Serial.print(": ");
        Serial.println((0 == sx1503.digitalRead((c - 0x30))) ? "LOW" : "HIGH");
        break;

      case 'c':  // Reconfigures the SX1503 as this sketch uses it.
        ret = init_sx1503();
        Serial.print("init_sx1503() returns ");
        Serial.println(ret, DEC);
        break;

      case 'i':
        sx1503.printDebug();
        break;

      case 'r':
        sx1503.printRegs();
        break;

      case 'S':   // Save the state into a buffer for later reconstitution.
        {
          uint8_t buf[SX1503_SERIALIZE_SIZE];
          uint8_t written = sx1503.serialize(buf, SX1503_SERIALIZE_SIZE);
          if (SX1503_SERIALIZE_SIZE == written) {
            for (unsigned int i = 0; i < SX1503_SERIALIZE_SIZE; i++) {
              Serial.print(buf[i], HEX);
              Serial.print(((i+1) % 16) ? " " : "\n");
            }
            Serial.println();
          }
          else {
            Serial.print("serialize() returns ");
            Serial.print(written);
            Serial.print(". Was expecting ");
            Serial.println(SX1503_SERIALIZE_SIZE);
          }
        }
        break;
    }
  }
  else if (sx1503.isrFired()) {
    // poll() makes everything happen. If you pass 255 for the IRQ pin on
    //   construction, this code should still work because isrFired() will
    //   always return true in that case.
    ret = sx1503.poll();
    if (0 != ret) {
      Serial.print("poll() returns error code ");
      Serial.println(ret, DEC);
    }
  }
  else {
    digitalWrite(LED0_PIN, HIGH);
  }
}

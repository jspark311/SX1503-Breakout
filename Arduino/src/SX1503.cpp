/*
*/

#include "SX1503.h"
#include <Wire.h>

/*******************************************************************************
*      _______.___________.    ___   .___________. __    ______     _______.
*     /       |           |   /   \  |           ||  |  /      |   /       |
*    |   (----`---|  |----`  /  ^  \ `---|  |----`|  | |  ,----'  |   (----`
*     \   \       |  |      /  /_\  \    |  |     |  | |  |        \   \
* .----)   |      |  |     /  _____  \   |  |     |  | |  `----.----)   |
* |_______/       |__|    /__/     \__\  |__|     |__|  \______|_______/
*
* Static members and initializers should be located here.
*******************************************************************************/

volatile static bool isr_fired = false;

/* Real register addresses */
static const uint8_t SX1503_REG_ADDR[31] = {
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
  0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
  0x10, 0x11, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25,
  0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0xAD
};



/*
* This is an ISR.
*/
void sx1503_isr() {
  isr_fired = true;
}



/*******************************************************************************
*   ___ _              ___      _ _              _      _
*  / __| |__ _ ______ | _ ) ___(_) |___ _ _ _ __| |__ _| |_ ___
* | (__| / _` (_-<_-< | _ \/ _ \ | / -_) '_| '_ \ / _` |  _/ -_)
*  \___|_\__,_/__/__/ |___/\___/_|_\___|_| | .__/_\__,_|\__\___|
*                                          |_|
* Constructors/destructors, class initialization functions and so-forth...
*******************************************************************************/

SX1503::SX1503(const uint8_t irq_pin, const uint8_t reset_pin) : _IRQ_PIN(irq_pin), _RESET_PIN(reset_pin) {
  for (uint8_t i = 0; i < sizeof(registers); i++) {
    registers[i] = 0;
  }
  for (uint8_t i = 0; i < sizeof(callbacks); i++) {
    callbacks[i]  = nullptr;
    priorities[i] = 0;
  }
}


SX1503::~SX1503() {
}



/*
* Dump this item to the dev log.
*/
void SX1503::printDebug(StringBuilder* output) {
  output->concat("---< SX1503 >---------------------------------------------------\n");
  output->concatf("RESET Pin:   %c\n", _RESET_PIN);
  output->concatf("IRQ Pin:     %c\n", _IRQ_PIN);
  output->concatf("isr_fired:   %c\n", isr_fired ? 'y' : 'n');
}


void SX1503::printRegs(StringBuilder* output) {
  for (uint8_t i = 0; i < sizeof(registers); i++) {
    output->concatf("\t%u: 0x%02x\n", i, registers[i]);
  }
}


int8_t SX1503::init() {
  _ll_pin_init();
  return reset();
}


int8_t SX1503::reset() {
  int8_t ret = -1;
  for (uint8_t i = 0; i < sizeof(registers); i++) {
    registers[i] = 0;
  }
  if (255 != _RESET_PIN) {
    digitalWrite(_RESET_PIN, false);
    delay(1);   // Datasheet says 300ns.
    digitalWrite(_RESET_PIN, true);
    delay(8);   // Datasheet says 7ms.
  }
  else {
    // TODO: Steamroll the registers with the default values.
  }
  // TODO: Wait on the IRQ pin to go high.
  ret = _write_register(SX1503_REG_ADVANCED, 0x02);  // IRQ clears on data read.
  return ret;
}


int8_t SX1503::poll() {
  int8_t ret = 0;
  if (isr_fired) {
    uint8_t b = registers[SX1503_REG_DATA_B];
    uint8_t a = registers[SX1503_REG_DATA_A];
    if (0 == _read_register(SX1503_REG_DATA_B, 2)) {
      uint8_t d = b ^ registers[SX1503_REG_DATA_B];
      if (d) {
        for (uint8_t i = 0; i < 8; i++) {
          if ((d >> i) & 1) {
            _invoke_pin_callback((i+8), ((registers[SX1503_REG_DATA_B] >> i) & 1));
          }
        }
      }
      d = a ^ registers[SX1503_REG_DATA_B];
      if (d) {
        for (uint8_t i = 0; i < 8; i++) {
          if ((d >> i) & 1) {
            _invoke_pin_callback(i, ((registers[SX1503_REG_DATA_A] >> i) & 1));
          }
        }
      }
    }
  }
  if (255 != _IRQ_PIN) {
    isr_fired = !digitalRead(_IRQ_PIN);
  }
  return ret;
}



int8_t SX1503::_write_register(uint8_t reg, uint8_t val) {
  int8_t return_value = -1;
  // No special safety measures are needed here.
  Wire.beginTransmission(SX1503_I2C_ADDR);
  Wire.write(reg);
  Wire.write(val);
  Wire.endTransmission();
  _set_shadow_value(reg, val);
  return_value = 0;
  return return_value;
}


int8_t SX1503::_read_register(uint8_t reg, uint8_t len) {
  int8_t return_value = 0;
  Wire.beginTransmission(SX1503_I2C_ADDR);
  Wire.send(reg);
  Wire.endTransmission(false);
  Wire.requestFrom(SX1503_I2C_ADDR, len);
  for (uint8_t i = 0; i < len; i++) {
    _set_shadow_value(reg + i, Wire.receive());
  }
  Wire.endTransmission();
  return return_value;
}



/*
* Setup the low-level pin details.
*/
int8_t SX1503::_ll_pin_init() {
  if (255 != _IRQ_PIN) {
    pinMode(_IRQ_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(_IRQ_PIN), SX1503_isr, FALLING);
  }
  if (255 != _RESET_PIN) {
    pinMode(_RESET_PIN, OUTPUT);
  }
  return 0;
}


int8_t SX1503::attachInterrupt(uint8_t pin, SX1503Callback cb, uint8_t condition) {
  pin &= 0x0F;
  int8_t ret = -1;
  if (16 > pin) {
    callbacks[pin]  = cb;
    priorities[pin] = 0;
    ret = 0;
  }
  return ret;
}


/*
* Returns the number of interrupts removed.
*/
int8_t SX1503::detachInterrupt(uint8_t pin) {
  pin &= 0x0F;
  int8_t ret = (nullptr == callbacks[pin]) ? 0 : 1;
  callbacks[pin]  = nullptr;
  priorities[pin] = 0;
  return ret;
}


/*
* Returns the number of interrupts removed.
*/
int8_t SX1503::detachInterrupt(SX1503Callback cb) {
  pin &= 0x0F;
  int8_t ret = 0;
  for (uint8_t i = 0; i < sizeof(callbacks); i++) {
    if (cb == callbacks[i]) {
      callbacks[i] = nullptr;
      priorities[i] = 0;
      ret++;
    }
  }
  return ret;
}


int8_t SX1503::_invoke_pin_callback(uint8_t pin, bool value) {
  int8_t ret = -1;
  pin &= 0x0F;
  if (nullptr != callbacks[pin]) {
    callbacks[pin](pin, value?1:0);
    ret = 0;
  }
  return ret;
}


void SX1503::_set_shadow_value(uint8_t reg, uint8_t val) {
  if (reg < 31) {  registers[reg] = val;   }
}

uint8_t SX1503::_get_shadow_value(uint8_t reg) {
  return (reg < 31) ? registers[reg] : 0;
}


/*
*/
uint8_t SX1503::digitalWrite(uint8_t pin, uint8_t value) {
  uint8_t ret = -2;
  if (pin < 16) {
    ret = 0;
    uint8_t reg0 = (pin < 8) ? SX1503_REG_DATA_A : SX1503_REG_DATA_B;
    uint8_t val0 = (pin < 8) ? registers[SX1503_REG_DATA_A] : registers[SX1503_REG_DATA_B];
    pin = pin & 0x07; // Restrict to 8-bits.
    uint8_t f = 1 << pin;
    val0 = (value) ? (val0 |= f) : (val0 &= ~f);
    if ((0 == ret) & (registers[reg0] != val0)) {  ret = _write_register(reg0, val0);   }
  }
  return ret;
}


/*
*/
uint8_t SX1503::digitalRead(uint8_t pin) {
  uint8_t ret = 0;
  if (pin < 8) {
    ret = (registers[SX1503_REG_DATA_A] >> pin) & 0x01;
  }
  else if (pin < 16) {
    ret = (registers[SX1503_REG_DATA_B] >> pin) & 0x01;
  }
  return ret;
}


int8_t SX1503::gpioMode(uint8_t pin, int mode) {
  uint8_t ret = -1;
  if (pin < 16) {
    ret = 0;
    bool in = true;
    bool pu = false;
    bool pd = false;
    switch (mode) {
      case OUTPUT_PULLDOWN:
        pd = true;
        in = false;
        break;
      case OUTPUT_PULLUP:
        pu = true;
        in = false;
        break;
      case OUTPUT:
        in = false;
        break;
      case INPUT_PULLUP:
        pu = true;
        break;
      case INPUT_PULLDOWN:
        pd = true;
        break;
      case INPUT:
      default:
        break;
    }

    uint8_t reg0 = (pin < 8) ? SX1503_REG_DIR_A : SX1503_REG_DIR_B;
    uint8_t reg1 = (pin < 8) ? SX1503_REG_PULLUP_A : SX1503_REG_PULLUP_B;
    uint8_t reg2 = (pin < 8) ? SX1503_REG_PULLDOWN_A : SX1503_REG_PULLDOWN_B;
    uint8_t reg3 = (pin < 8) ? SX1503_REG_IRQ_MASK_A : SX1503_REG_IRQ_MASK_B;
    uint8_t val0 = (pin < 8) ? registers[SX1503_REG_DIR_A] : registers[SX1503_REG_DIR_B];
    uint8_t val1 = (pin < 8) ? registers[SX1503_REG_PULLUP_A] : registers[SX1503_REG_PULLUP_B];
    uint8_t val2 = (pin < 8) ? registers[SX1503_REG_PULLDOWN_A] : registers[SX1503_REG_PULLDOWN_B];
    uint8_t val3 = (pin < 8) ? registers[SX1503_REG_IRQ_MASK_A] : registers[SX1503_REG_IRQ_MASK_B];
    pin = pin & 0x07; // Restrict to 8-bits.
    uint8_t f = 1 << pin;

    // Pin being set as an input means we need to unmask the interrupt.
    val0 = (in) ? (val0 |= f) : (val0 &= ~f);
    val1 = (pu) ? (val1 |= f) : (val1 &= ~f);
    val2 = (pd) ? (val2 |= f) : (val2 &= ~f);
    val3 = (in) ? (val3 &= ~f) : (val3 |= f);

    if ((0 == ret) & (registers[reg0] != val0)) {  ret = _write_register(reg0, val0);   }
    if ((0 == ret) & (registers[reg1] != val1)) {  ret = _write_register(reg1, val1);   }
    if ((0 == ret) & (registers[reg2] != val2)) {  ret = _write_register(reg2, val2);   }
    if ((0 == ret) & (registers[reg3] != val3)) {  ret = _write_register(reg3, val3);   }
  }
  return ret;
}

/*
*/

#ifndef __SX1503_DRIVER_H__
#define __SX1503_DRIVER_H__

#include <Arduino.h>
#include <Wire.h>


#define SX1503_I2C_ADDR           0x20  // Not configurable.
#define SX1503_SERIALIZE_VERSION  0x01  // Version code for serialized states.
#define SX1503_SERIALIZE_SIZE       36


/* Class flags. */
#define SX1503_FLAG_PRESERVE_STATE   0x0001
#define SX1503_FLAG_PINS_CONFD       0x2000
#define SX1503_FLAG_INITIALIZED      0x4000
#define SX1503_FLAG_FROM_BLOB        0x8000

#define SX1503_FLAG_SERIAL_MASK      0x000F  // Only these bits are serialized.


/* These are the i2c register indicies. NOT their addresses. */
// TODO: Convert to enum so confusion doesn't happen.
#define SX1503_REG_DATA_B         0x00  //
#define SX1503_REG_DATA_A         0x01  //
#define SX1503_REG_DIR_B          0x02  //
#define SX1503_REG_DIR_A          0x03  //
#define SX1503_REG_PULLUP_B       0x04  //
#define SX1503_REG_PULLUP_A       0x05  //
#define SX1503_REG_PULLDOWN_B     0x06  //
#define SX1503_REG_PULLDOWN_A     0x07  //
#define SX1503_REG_IRQ_MASK_B     0x08  //
#define SX1503_REG_IRQ_MASK_A     0x09  //
#define SX1503_REG_SENSE_H_B      0x0A  //
#define SX1503_REG_SENSE_H_A      0x0B  //
#define SX1503_REG_SENSE_L_B      0x0C  //
#define SX1503_REG_SENSE_L_A      0x0D  //
#define SX1503_REG_IRQ_SRC_B      0x0E  //
#define SX1503_REG_IRQ_SRC_A      0x0F  //
#define SX1503_REG_EVENT_STAT_B   0x10  //
#define SX1503_REG_EVENT_STAT_A   0x11  //
#define SX1503_REG_PLD_MODE_B     0x12  //
#define SX1503_REG_PLD_MODE_A     0x13  //
#define SX1503_REG_PLD_TABLE_0B   0x14  //
#define SX1503_REG_PLD_TABLE_0A   0x15  //
#define SX1503_REG_PLD_TABLE_1B   0x16  //
#define SX1503_REG_PLD_TABLE_1A   0x17  //
#define SX1503_REG_PLD_TABLE_2B   0x18  //
#define SX1503_REG_PLD_TABLE_2A   0x19  //
#define SX1503_REG_PLD_TABLE_3B   0x1A  //
#define SX1503_REG_PLD_TABLE_3A   0x1B  //
#define SX1503_REG_PLD_TABLE_4B   0x1C  //
#define SX1503_REG_PLD_TABLE_4A   0x1D  //
#define SX1503_REG_ADVANCED       0x1E  //

/* An optional IRQ handler function that the application can pass in. */
typedef void (*SX1503Callback)(uint8_t pin, uint8_t level);

/* Blend in with the board's API and extend if necessary. */
#if !defined(INPUT_PULLDOWN)
  #define INPUT_PULLDOWN     13  // TODO: Arbitrary. Read more...
#endif
#if !defined(OUTPUT_PULLUP)
  #define OUTPUT_PULLUP      14  // TODO: Arbitrary. Read more...
#endif
#if !defined(OUTPUT_PULLDOWN)
  #define OUTPUT_PULLDOWN    15  // TODO: Arbitrary. Read more...
#endif


/*
* Driver class.
*/
class SX1503 {
  public:
    SX1503(const uint8_t irq_pin, const uint8_t reset_pin);
    SX1503(const uint8_t* buf, const unsigned int len);  // Takes serialized state as args.
    ~SX1503();

    inline int8_t init() {  return init(_bus);  };
    int8_t init(TwoWire*);
    int8_t reset();
    int8_t poll();
    bool isrFired();
    int8_t refresh();

    // Basic usage as pins...
    int8_t  gpioMode(uint8_t pin, int mode);
    int8_t  digitalWrite(uint8_t pin, uint8_t value);
    uint8_t digitalRead(uint8_t pin);

    // Interrupt and callback management...
    int8_t  attachInterrupt(uint8_t pin, SX1503Callback, uint8_t condition);
    int8_t  detachInterrupt(uint8_t pin);
    int8_t  detachInterrupt(SX1503Callback);

    // Advanced usage...
    int8_t  setPLD();  // TODO: Define API for this feature.
    int8_t  useBoost(bool enable);

    // No NVM on this part, so these fxns help do init in a single step.
    uint8_t serialize(uint8_t* buf, unsigned int len);
    int8_t  unserialize(const uint8_t* buf, const unsigned int len);

    inline bool initialized() {  return _sx_flag(SX1503_FLAG_INITIALIZED);  };
    inline bool preserveOnDestroy() {
      return _sx_flag(SX1503_FLAG_PRESERVE_STATE);
    };
    inline void preserveOnDestroy(bool x) {
      _sx_set_flag(SX1503_FLAG_PRESERVE_STATE, x);
    };

    // Debugging fxns...
    void printDebug();
    void printRegs();


  private:
    const uint8_t  _IRQ_PIN;
    const uint8_t  _RESET_PIN;
    uint16_t       _flags = 0;
    TwoWire*       _bus   = nullptr;
    uint8_t        priorities[16];
    SX1503Callback callbacks[16];
    uint8_t        registers[31];

    int8_t _invoke_pin_callback(uint8_t pin, bool value);

    int8_t  _write_register(uint8_t reg, uint8_t val);
    int8_t  _write_register(uint8_t reg, uint8_t* buf, uint8_t len);
    int8_t  _read_register(uint8_t reg, uint8_t len);
    int8_t  _ll_pin_init();

    inline bool _from_blob() {   return _sx_flag(SX1503_FLAG_FROM_BLOB); };

    /* Flag manipulation inlines */
    inline uint16_t _sx_flags() {                return _flags;           };
    inline bool _sx_flag(uint16_t _flag) {       return (_flags & _flag); };
    inline void _sx_clear_flag(uint16_t _flag) { _flags &= ~_flag;        };
    inline void _sx_set_flag(uint16_t _flag) {   _flags |= _flag;         };
    inline void _sx_set_flag(uint16_t _flag, bool nu) {
      if (nu) _flags |= _flag;
      else    _flags &= ~_flag;
    };
};

#endif   // __SX1503_DRIVER_H__

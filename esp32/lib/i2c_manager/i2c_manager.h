// i2c_manager.h

#include <Wire.h>

namespace I2CManager{
    /**
     * @brief Initialize I2C bus
     * @param sda SDA pin (default: 21 for ESP32)
     * @param scl SCL pin (default: 22 for ESP32)
     * @return true is init is succesful
     */
    bool begin(uint8_t sda=21, uint8_t scl=22);

    void reset();
    
    bool deviceExists(uint8_t addr);

    // scan all the devices on the bus
    void scanBus();

    // Getter for wire
    TwoWire &getWire();
}
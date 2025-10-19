// bh1750.h

#ifndef BH1750_H
#define BH1750_H

#include <Wire.h>

class BH1750{
    public:
        // Initialize the sensor over I2C using the TwoWire instance and address
        bool begin(TwoWire &wire=Wire, uint8_t addr=0x23);

        /**
         * - Moonless night       : 0.1 lx
         * - Fullmoon             : 0.1-10 lx
         * - Heavy clouds or rain : 100-1000 lx
         * - Sunrise/Sunset       : 40-400 lx
         * - Overcast day         : 1,000-10,000 lx
         * - Sunny noon           : 10,000-100,000 lx
         */
        float readAmbientLight();   // returns 1-65535 lx

    private:
        TwoWire* _wire; // I2C instance
        uint8_t _addr;  // I2C address
};
#endif
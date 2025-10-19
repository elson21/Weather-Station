// i2c_manager.cpp

#include <Arduino.h>
#include "i2c_manager.h"

namespace I2CManager{

    
    static uint8_t _sda = 21 ;
    static uint8_t _scl = 22 ;

    bool begin(uint8_t sda, uint8_t scl){
        _sda = sda;
        _scl = scl;

        Wire.begin(_sda, _scl);

        return true;
    }


    void reset(){
        Wire.end();
        delay(10);
        Wire.begin(_sda, _scl);
    }


    bool deviceExists(uint8_t addr){
        /**
         * Value    Meaning
         * 0        Success - device ACK'd
         * 1        Data too long
         * 2        NACK on address (no device)
         * 3        NACK on data
         * 4        Other error
         */

        Wire.beginTransmission(addr);
        
        uint8_t error = Wire.endTransmission(); // return value stored in error

        return (error == 0);
    }


    void scanBus(){
        Serial.println("Scanning...");

        for (uint8_t addr = 1; addr <= 127; addr++){    // I2C uses a 7-bit address ranger
            if(deviceExists(addr)){
                Serial.print("Found: 0x");
                Serial.println(addr, HEX);
            }
        }

        Serial.println("Scan complete.");
    }

    TwoWire &getWire(){
        return Wire;
    }
}
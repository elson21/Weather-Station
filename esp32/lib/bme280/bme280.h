// bme280.h

#ifndef BME280_H
#define BME280_H

#include <Wire.h>

class BME280{
    public:
        // initialize the sensor over I2C using the TwoWire instance and address
        bool begin(TwoWire &wire=Wire, uint8_t addr=0x76);

        float readTemperature();    // temperature in °C
        float readPressure();   // pressure in Pa
        float readHumidity();   // humidity in %RH

    private:
        TwoWire* _wire; // I2C bus isntance
        uint8_t _addr; // I2C address

        // calibration params
        // temperature
        uint16_t dig_T1;
        int16_t dig_T2;
        int16_t dig_T3;

        // pressure
        uint16_t dig_P1;
        int16_t dig_P2;
        int16_t dig_P3;
        int16_t dig_P4;
        int16_t dig_P5;
        int16_t dig_P6;
        int16_t dig_P7;
        int16_t dig_P8;
        int16_t dig_P9;

        // humidity
        uint8_t dig_H1;
        uint16_t dig_H2;
        uint8_t dig_H3;
        uint16_t dig_H4;
        uint16_t dig_H5;
        uint8_t dig_H6;
        
        // temperature compensation
        int32_t t_fine;
};
#endif
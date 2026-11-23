/*  created with example code .ino from 14point7 for Spartan-2-OEM-I2C
 *  Gaetan Caron may 2020
 *  Spartan2 library ver 1.0.2
 *  If you need to force/reset Spartan-2-OEM-I2C's I2C Address, please read the last page of the Spartan_2_OEM_I2C Manual
 *
 * This Code example has been tested with ESP32 but can work with arduino board
 * SDA and SCL lines require 4.7k pullups
 * arduino SDA to Spartan-2-OEM-I2C SDA
 * arduino SCL to Spartan-2-OEM-I2C SCL
 * ESP32 default I2C: SDA = GPIO21 SCL = GPIO22
 * Data from Spartan-2-OEM-I2C is displayed on the Arduino IDE Serial Monitor
 *
 */
#ifndef SPARTAN2_H
#define SPARTAN2_H

#include <stdint.h>
#include <Wire.h>

#ifndef Spartan2_DEBUG
#define Spartan2_DEBUG 0
#endif

// The lookup tables live in Spartan2.cpp so they are stored only once in flash.
extern const unsigned int Pump_Current16_to_Lambda[];
extern const unsigned int Ri_to_Temperature_C[];

class Spartan2
{
public:
    Spartan2(uint8_t addr, float K, float AfrStoich);

    void begin(void);
    void read(void);
    void writeAddr(uint8_t addr);

    // 1 = no (or incomplete) answer from the sensor, 0 = data is valid.
    uint8_t No_I2C_Response_Flag = 1;

    struct Processed_Data_Struct
    {
        uint8_t I2C_Addr;
        uint8_t ID;
        float Lambda;
        uint16_t Temperature_C;
        uint8_t Status;
        float Ma;
        float Afr;
        float O2;   // fraction (0..1), multiply by 100 to get a percentage
    };
    Processed_Data_Struct Get; // This struct holds the final processed data

private:
    #define Pump_Current16_to_Lambda_Start 0  // Index = Pump_Current16 - Pump_Current16_to_Lambda_Start
    #define Pump_Current16_to_Lambda_Size 409 // Size of Pump_Current16_to_Lambda array
    #define Ri_to_Temperature_C_Start 113     // Index = Ri - Ri_to_Temperature_C_Start
    #define Ri_to_Temperature_C_Size 75       // Size of Ri_to_Temperature_C array

    struct _I2C_Struct
    {
        uint8_t _I2C_Addr;
        uint8_t _ID8;
        uint16_t _Pump_Current16;
        uint8_t _Ri;
        uint8_t _Status8;
    };
    _I2C_Struct _I2C_Data; // I2C data in the same form as on the Spartan 2

    uint8_t _I2C_Address;
    uint8_t _Val_Array[10];
    uint8_t _temp;
    int _i = 0;
    float _K;
    float _AfrStoich;
    uint8_t Xbyte = 6;
};
#endif

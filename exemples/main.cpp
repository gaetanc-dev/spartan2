/*  created with example code from 14point7 for Spartam-2-OEM-I2C
 *  Gaetan Caron
 *  may 2020
 *  Spartan2 library
 *  If you need to force/reset Spartam-2-OEM-I2C's I2C Address, please read the last page of the Spartam_2_OEM_I2C Manual  
 * 
 * This Code example has been tested with ESP32 but can work with AVR  
 * SDA and SLC lines require 4.7k pullups
 * arduino SDA to Spartam-2-OEM-I2C SDA
 * arduino SCL to Spartam-2-OEM-I2C SCL
 * ESP32 default I2C: SDA = GPIO22 SCL = GPIO21
 * Data from Spartam-2-OEM-I2C is diplayed on the Arduino IDE Serial Monitor
 *
 */

#include <Arduino.h>
#include <Spartan2.h>
uint8_t I2C_Address = 0x01;
float K = 3.73;
float AfrStoich = 14.7;

Spartan2 O2(I2C_Address, K, AfrStoich);

void setup()
{
    Serial.begin(115200);
    O2.begin();
}

void loop()
{
    O2.read();
    if (O2.No_I2C_Response_Flag == 1)
    {
        Serial.println("no connection to spartan 2");
    }
    if (O2.No_I2C_Response_Flag == 0)
    {
        Serial.print("Communicating using I2C Address ");
        Serial.println(I2C_Address);
        Serial.print("Spartan 2 onboard I2C Address is set to ");
        Serial.println(O2.Get.I2C_Addr);
        Serial.print("ID=");
        Serial.println(O2.Get.ID);
        Serial.print("Lambda=");
        Serial.println(O2.Get.Lambda);
        Serial.print("Temperature[C]=");
        Serial.println(O2.Get.Temperature_C);
        Serial.print("Status=");
        Serial.println(O2.Get.Status);
        Serial.print("cern Ma=");
        Serial.println(O2.Get.Ma, 4);
        Serial.print("O2%=");
        Serial.println(O2.Get.O2 * 100);
        Serial.print("Air/Fuel ratio=");
        Serial.println(O2.Get.Afr);
        Serial.println();
        Serial.println();
    }
    delay(1000); //Change this to increase or decrease serial update rate
}
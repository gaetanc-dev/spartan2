/*  Spartan2 library example - basic I2C readout
 *
 *  Tested with ESP32, also works on AVR boards.
 *  SDA and SCL lines require 4.7k pullups.
 *    Arduino : SDA/SCL board pins -> Spartan-2-OEM-I2C SDA/SCL
 *    ESP32   : default I2C SDA = GPIO21, SCL = GPIO22
 *
 *  Data is printed to the Arduino IDE Serial Monitor.
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
        Serial.println("No connection to Spartan 2");
    }
    else
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
        Serial.print("Pump current [mA]=");
        Serial.println(O2.Get.Ma, 4);
        Serial.print("O2 [%]=");
        Serial.println(O2.Get.O2 * 100);
        Serial.print("Air/Fuel ratio=");
        Serial.println(O2.Get.Afr);
        Serial.println();
        Serial.println();
    }
    delay(1000); // Change this to increase or decrease the serial update rate
}

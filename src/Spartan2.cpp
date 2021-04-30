/*  created with example code from 14point7 for Spartam-2-OEM-I2C
 *  Gaetan Caron may 2020
 *  version 1.0.1
 *  Spartan2 library
 *  If you need to force/reset Spartam-2-OEM-I2C's I2C Address, please read the last page of the Spartam_2_OEM_I2C Manual  
 * 
 * This Code example has been tested with ESP32 but can work with arduino board 
 * SDA and SLC lines require 4.7k pullups
 * arduino SDA to Spartam-2-OEM-I2C SDA
 * arduino SCL to Spartam-2-OEM-I2C SCL
 * ESP32 default I2C: SDA = GPIO22 SCL = GPIO21
 * Data from Spartam-2-OEM-I2C is diplayed on the Serial Monitor
 *
 */

#if ARDUINO >= 100
#include "Arduino.h"
#else
extern "C" {
#include "WConstants.h"
}
#endif
#include <Spartan2.h>
#include <Wire.h>
#if defined ESP32
  #include <pgmspace.h>
#elif defined ESP8266
  #include <pgmspace.h>
#else
  #include <avr/pgmspace.h>
#endif

Spartan2::Spartan2(uint8_t addr,float K,float AfrStoich)  //constructor pass adress k ratio and stoich level
{
  _I2C_Address = addr;
  _K = K;
  _AfrStoich = AfrStoich;
  #if (Spartan2_DEBUG == 1)
    Serial.print("I2C_Address = "); Serial.println(_I2C_Address, DEC);
    Serial.print("K = "); Serial.println(_K, DEC);
    Serial.print("AfrStoich = "); Serial.println(_AfrStoich, 2);
  #endif
  
}

void Spartan2::begin() {
  Wire.begin(); // Initialize I2C
  #if (Spartan2_DEBUG == 1)
    Serial.println("begining Spartan2 cpp 36");
  #endif

}

void Spartan2::read(void) 
{
  

  Wire.beginTransmission(_I2C_Address); //Setup communication with device @ I2C Address = 1 
  Wire.write(0); //This will tell Spartan that OEM that we want to start a read from Memory Address 0
  Wire.endTransmission();     // end transmission
  Wire.requestFrom(_I2C_Address, Xbyte); //Read 6 Bytes from Device with I2C Address = I2C_Address
  _i=0;
  No_I2C_Response_Flag=1;
  while (Wire.available()) //Dump the I2C data into an Array
  {
    _Val_Array[_i] = Wire.read();
    No_I2C_Response_Flag=0;
    _i++;
  }
      
  _I2C_Data._I2C_Addr=_Val_Array[0];
  _I2C_Data._ID8=_Val_Array[1];
  _I2C_Data._Pump_Current16=(_Val_Array[2]<<8) + _Val_Array[3];
  _I2C_Data._Ri=_Val_Array[4];
  _I2C_Data._Status8=_Val_Array[5];
    
  Get.I2C_Addr=_I2C_Data._I2C_Addr;
  Get.ID=_I2C_Data._ID8;

  //pump_current16_to lambda ****************
  _i=_I2C_Data._Pump_Current16-Pump_Current16_to_Lambda_Start;
  // make sure array index is within array bounds
  if (_i<0)
  {
    _i=0;
  }
  if (_i>(Pump_Current16_to_Lambda_Size-1))
  {
    _i=Pump_Current16_to_Lambda_Size-1;
  }
  Get.Lambda=((float)pgm_read_word_near(Pump_Current16_to_Lambda+_i))/1000; //use Pump_Current16 as index to Pump_Current16_to_Lambda lookup table and then Divide by 1000 to get actual Lambda
  Get.Ma=((_I2C_Data._Pump_Current16-258)*0.0160); //convert Pump_current to ma
  Get.O2= 3 * (Get.Lambda - 1) / (1 + 3 * _K * Get.Lambda); //convert lambda to %O2
  Get.Afr= Get.Lambda * _AfrStoich; //comvert lambda to afr
  //Ri to temperature celcius   
  _i=_I2C_Data._Ri-Ri_to_Temperature_C_Start;// Index has to be justified to the right by Ri_to_Temperature_C_Start
   // make sure array index is within array bounds
  if (_i<0)
  {
    _i=0;
  }
  if (_i>(Ri_to_Temperature_C_Size-1))
  {
    _i=Ri_to_Temperature_C_Size-1;
  }
  Get.Temperature_C=pgm_read_word_near(Ri_to_Temperature_C+_i); //Use the Justified Ri as index to Ri_to_Temaperature_C lookup table to get get Sensor Temperature[C]
  Get.Status=_I2C_Data._Status8;
    #if (Spartan2_DEBUG == 1)
    if (No_I2C_Response_Flag == 1)
    {
      Serial.println("no answer from spartan2 board");
      Serial.println("CPP 99");
    }
    else
    {
    Serial.println("data processed:");
    Serial.print("spartan2 address : ");
    Serial.println(Get.I2C_Addr,DEC);
    Serial.print("firmware ; ");
    Serial.println(Get.ID);
    Serial.print("Afr : ");
    Serial.println(Get.Afr, 2);
    Serial.print("Lambda : ");
    Serial.println(Get.Lambda, 2);
    Serial.print("Ma : ");
    Serial.println(Get.Ma, 4);
    Serial.print("O2% : ");
    Serial.println(Get.O2, 2);
    Serial.println("CPP 115");
    }
    
  

    

  #endif

}

void Spartan2::writeAddr(uint8_t addr)
{
  _temp = addr;
  Wire.beginTransmission(_I2C_Address); //Setup communication with device @ I2C Address 
  Wire.write(0); //This will tell Spartan that OEM that we want to start a read from Memory Address 0
  Wire.write(_temp); //write New address to Spartan 2 OEM
  Wire.endTransmission();
  _I2C_Address = _temp;
  #if (Spartan2_DEBUG == 1)
    Serial.print("new address writed is :");
    Serial.println(_I2C_Address);
    Serial.println("note it to future use")
    Serial.println("CPP 136");
  #endif

        
}
 

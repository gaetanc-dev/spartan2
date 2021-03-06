/*  created with example code .ino from 14point7 for Spartam-2-OEM-I2C
 *  Gaetan Caron may 2020
 *  Spartan2 library ver 1.0.1
 *  If you need to force/reset Spartam-2-OEM-I2C's I2C Address, please read the last page of the Spartam_2_OEM_I2C Manual  
 * 
 * This Code example has been tested with ESP32 but can work with arduino board 
 * SDA and SLC lines require 4.7k pullups
 * arduino SDA to Spartam-2-OEM-I2C SDA
 * arduino SCL to Spartam-2-OEM-I2C SCL
 * ESP32 default I2C: SDA = GPIO22 SCL = GPIO21
 * Data from Spartam-2-OEM-I2C is diplayed on the Arduino IDE Serial Monitor
 *
 */
#ifndef Spartan2
#define Spartan2 .h

#include <Wire.h>
#define Spartan2_DEBUG 0

const PROGMEM unsigned int Pump_Current16_to_Lambda[] =
    {
        376, 379, 381, 384, 387, 390, 393, 395, 398, 401, 403, 406, 409,
        411, 414, 417, 419, 422, 424, 427, 430, 432, 435, 437, 440, 442,
        445, 447, 450, 452, 455, 457, 460, 462, 464, 467, 469, 472, 474,
        476, 479, 481, 483, 486, 488, 490, 493, 495, 497, 499, 502, 504,
        506, 508, 511, 513, 515, 517, 520, 522, 524, 526, 528, 530, 533,
        535, 537, 539, 541, 543, 546, 548, 550, 552, 554, 556, 558, 560,
        562, 564, 567, 569, 571, 573, 575, 577, 579, 581, 583, 585, 587,
        589, 591, 593, 595, 598, 600, 602, 604, 606, 608, 610, 612, 614,
        616, 618, 620, 622, 624, 626, 628, 630, 632, 634, 636, 638, 640,
        642, 644, 646, 648, 650, 653, 655, 657, 659, 661, 663, 665, 667,
        669, 671, 673, 675, 677, 679, 681, 684, 686, 688, 690, 692, 694,
        696, 698, 700, 703, 705, 707, 709, 711, 713, 715, 718, 720, 722,
        724, 726, 729, 731, 733, 735, 737, 740, 742, 744, 746, 749, 751,
        753, 756, 758, 760, 763, 765, 767, 770, 772, 774, 777, 779, 781,
        784, 786, 789, 791, 794, 796, 799, 801, 804, 806, 809, 811, 814,
        816, 819, 821, 824, 827, 829, 832, 834, 837, 840, 842, 845, 848,
        851, 853, 856, 859, 862, 864, 867, 870, 873, 876, 879, 881, 884,
        887, 890, 893, 896, 899, 902, 905, 908, 911, 914, 917, 920, 923,
        927, 930, 933, 936, 939, 942, 946, 949, 952, 955, 959, 962, 965,
        969, 972, 976, 979, 982, 986, 989, 993, 996, 1000, 1007, 1015,
        1023, 1031, 1039, 1047, 1056, 1064, 1073, 1082, 1090, 1099, 1108,
        1118, 1127, 1136, 1146, 1156, 1166, 1176, 1186, 1196, 1207, 1217,
        1228, 1239, 1250, 1262, 1273, 1285, 1297, 1309, 1321, 1334, 1346,
        1359, 1372, 1386, 1399, 1413, 1427, 1442, 1456, 1471, 1486, 1502,
        1517, 1533, 1550, 1566, 1583, 1600, 1618, 1636, 1654, 1673, 1692,
        1711, 1731, 1752, 1772, 1794, 1815, 1837, 1860, 1883, 1907, 1931,
        1956, 1981, 2007, 2033, 2061, 2088, 2117, 2146, 2176, 2207, 2239,
        2271, 2305, 2339, 2374, 2411, 2448, 2486, 2526, 2567, 2608, 2652,
        2696, 2742, 2790, 2839, 2889, 2942, 2996, 3052, 3110, 3171, 3233,
        3298, 3365, 3435, 3508, 3584, 3663, 3745, 3831, 3920, 4014, 4112,
        4215, 4323, 4436, 4555, 4680, 4812, 4952, 5099, 5255, 5420, 5596,
        5783, 5983, 6196, 6425, 6670, 6935, 7220, 7530, 7866, 8232, 8634,
        9076, 9564, 10106, 10713, 11394, 12167, 13050, 14068, 15256, 16659,
        18342, 20399, 22967, 26268, 30664, 36811, 46012, 61299};

//lookup table to convert Ri to Sensor Temperature[C]
const PROGMEM unsigned int Ri_to_Temperature_C[] =
    {
        820, 818, 817, 815, 814, 813, 811, 810, 808, 807, 805, 804, 803, 801, 800,
        799, 798, 796, 795, 794, 792, 791, 790, 789, 788, 786, 785, 784, 783, 782,
        781, 780, 779, 777, 776, 775, 774, 773, 772, 771, 770, 769, 768, 767, 766,
        765, 764, 763, 762, 761, 760, 759, 758, 758, 757, 756, 755, 754, 753, 752,
        751, 751, 750, 749, 748, 747, 746, 745, 745, 744, 743, 742, 741, 741, 740};

class Spartan2
{
public:
    Spartan2(uint8_t addr, float K, float AfrStoich);

    void begin(void);
    void read(void);
    void writeAddr(uint8_t addr);

   

    uint8_t No_I2C_Response_Flag = 1;

    struct Processed_Data_Struct
    {
        uint8_t I2C_Addr;
        uint8_t ID;
        float Lambda;
        unsigned int Temperature_C;
        uint8_t Status;
        float Ma;
        float Afr;
        float O2;
    };
    Processed_Data_Struct Get; //This Struct hold the final Processed Data

private:
    #define Pump_Current16_to_Lambda_Start 0  // To get index for Pump_Current16_to_Lambda Array for we take Pump_Current16 and subtract Pump_Current16_to_Lambda_Start
    #define Pump_Current16_to_Lambda_Size 409 //size of Pump_Current16_to_Lambda Array
    #define Ri_to_Temperature_C_Start 113     // To get index for Ri_to_Temperature_C Array for we take Ri and subtract Ri_to_Temperature_C_Start
    #define Ri_to_Temperature_C_Size 75       //size of Ri_to_Temperature_C Array
 
    struct _I2C_Struct
    {
        uint8_t _I2C_Addr;
        uint8_t _ID8;
        int _Pump_Current16;
        uint8_t _Ri;
        uint8_t _Status8;
    };
    _I2C_Struct _I2C_Data; //This Struct holds the I2C data in the same form that it is on Spartan 2

    uint8_t _I2C_Address;
    uint8_t _Val_Array[10];
    uint8_t _temp;
    unsigned int _i = 0;
    float _K ;
    float _AfrStoich;
    uint8_t Xbyte = 6;
    uint8_t stopOn = 1;
};
#endif
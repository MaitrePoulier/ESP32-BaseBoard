#ifndef __Batt_H_
    #define __Batt_H_

    #define TABLE_SIZE 12
    // Structure to define lookup points
    struct VoltageToSoC {
        float voltage;
        float percentage;
    };

    const VoltageToSoC socTable[TABLE_SIZE] = {
        {3.65, 100.0}, // 1500.0 mAh - Full charge ceiling (charging cut-off)
        {3.40,  99.0}, // 1485.0 mAh - Rested surface charge settles here quickly
        {3.35,  95.0}, // 1425.0 mAh - Upper knee entry
        {3.32,  90.0}, // 1350.0 mAh 
        {3.30,  80.0}, // 1200.0 mAh - Plateau begins
        {3.28,  70.0}, // 1050.0 mAh
        {3.26,  40.0}, //  600.0 mAh
        {3.25,  30.0}, //  450.0 mAh - Plateau ends
        {3.22,  20.0}, //  300.0 mAh - Lower knee entry (Steep drop-off begins)
        {3.15,  14.0}, //  210.0 mAh
        {3.00,   9.0}, //  135.0 mAh - Critical low
        {2.50,   0.0}  //    0.0 mAh - Hard discharge cut-off
    };

    float getBatteryPercentage(float);

#endif
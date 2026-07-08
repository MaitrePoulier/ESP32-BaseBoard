#ifndef __Batt_H_
    #define __Batt_H_

    #define TABLE_SIZE 15
    // Structure to define lookup points
    struct VoltageToSoC {
        float voltage;
        float percentage;
    };

    const VoltageToSoC socTable[TABLE_SIZE] = {
        {3.65, 100.0},
        {3.61,  99.0},
        {3.46,  95.0},
        {3.32,  90.0},
        {3.31,  80.0},
        {3.30,  70.0},
        {3.29,  60.0},
        {3.28,  50.0},
        {3.27,  40.0},
        {3.25,  30.0},
        {3.22,  20.0},
        {3.20,  17.0},
        {3.12,  14.0},
        {3.00,   9.0},
        {2.50,   0.0},
    };

    float getBatteryPercentage(float);

#endif
#ifndef __Cellular_H_
    #define __Cellular_H_

    #include <Arduino.h>
    #include <TFT_eSPI.h> 

    #define GRIDX 160
    #define GRIDY 106
    #define CELLXY 3

    //Number of generations
    #define NUMGEN 600
    
    void game(const char*);

#endif
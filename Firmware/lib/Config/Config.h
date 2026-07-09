#ifndef _GENERAL_H_
#define _GENERAL_H_
    #include "esp_random.h"
    #include <TFT_eSPI.h> 

    #define I2S_WS      26   // LRCK (WSEL)
    #define I2S_SD      25   // DATA OUT
    #define I2S_SCK     27    // BCLK 

    #define BattPin     39

    #define SCREEN_WIDTH 480 
    #define SCREEN_HEIGHT 320 


    //Define the random number source
    //static inline int32_t console_random(void){return rng_read();}
    inline int32_t console_random(void){return esp_random();}  //if not on STM32F753

    extern TFT_eSPI tft;

#endif

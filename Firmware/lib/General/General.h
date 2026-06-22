#ifndef _GENERAL_H_
#define _GENERAL_H_
    #include "esp_random.h"

    //Mettre en commentaire pour l'évalboard
    #define PROPEL
    
    #ifdef PROPEL
        #define CS          34
        //#define MISO        37   
        //#define MOSI        35  
        #define CLK         36  
        #define I2S_WS      46   // LRCK (WSEL)
        #define I2S_SD      45   // DATA OUT
        #define I2S_SCK     3    // BCLK 
        #define UART_RX     44
        #define UART_TX     43
        #define UART_RTS    15
        #define UART_CTS    16
    #else
        #define CS         SS
    #endif

    //Define the random number source
    //static inline int32_t console_random(void){return rng_read();}
    inline int32_t console_random(void){return esp_random();}  //if not on STM32F753

    //SD declaration
    #define SD_FAT_TYPE 3
    // default CS pin is SS
    #define SD_CS_PIN CS
    #define S3_PWR_en 5

    /*
    enum    errCode{
        error = 0,
        valid = 1,
        commandCompleted = 2
    };*/



#endif

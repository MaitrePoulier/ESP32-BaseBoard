#ifndef _GENERAL_H_
#define _GENERAL_H_

    //Mettre en commentaire pour l'évalboard
    #define PROPEL
    
    #ifdef PROPEL
        #define CS         34
        #define MISO       37   
        #define MOSI       35  
        #define CLK        36   
    #else
        #define CS         SS
    #endif



    //SD declaration
    #define SD_FAT_TYPE 3
    // default CS pin is SS
    #define SD_CS_PIN SS
    #define S3_PWR_en 5

    /*
    enum    errCode{
        error = 0,
        valid = 1,
        commandCompleted = 2
    };*/



#endif

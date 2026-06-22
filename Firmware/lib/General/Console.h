#ifndef _CONSOLE_H_
#define _CONSOLE_H_
    #define FIRST_TIME  99

    #include <Arduino.h>
    #include <string.h>
    #include "General.h"

    enum Command{
        CMD_noCommand,
        CMD_History,
        CMD_Help,
        CMD_SD,
        CMD_Invalid,
        Error
    };
    
    Command getInput(String *, char c);
    void displayHistory(void);
    void displayIntro(void);
    void displayHelp(void);

#endif
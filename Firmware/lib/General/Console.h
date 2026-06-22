#ifndef _CONSOLE_H_
    #define _CONSOLE_H_

    #include <stdint.h>
    #include <stdio.h>
    #include "ainsi-color-codes.h"

    #define FIRST_TIME 99

    typedef enum Command{
        CMD_noCommand,
        CMD_Invalid,
        Error
    }command_t;

    typedef void (*console_func_t)(char *arg);

    typedef struct {
        const char * parser;    // <--- Doit être un pointeur (char *)
        const char * help;       // <--- Doit être un pointeur (char *)
        int nb_arg;
        console_func_t func;
    } console_t;
        
    // Retourne l'index dans le tableau console_t, ou -1 si rien n'est trouvé
    command_t console_getInput(char input_char, char * argument, const console_t * console,int * cmd_index);

    void console_displayHistory(void);
    void console_displayIntro(void);
    void console_displayHelp(console_t * );
    void console_test(void);

#endif
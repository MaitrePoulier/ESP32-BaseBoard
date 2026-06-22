#include <Arduino.h>
#include <string.h>
#include "General.h"
#include "Console.h"
#include <wifi.h>
#include "MyWifi.h"
#include "s3.h"


/********************* Console **************************************/
// Handlers simples
extern console_t console[13];
void handler_help(char *)      { console_displayHelp(console); }
void handler_history(char *)   { console_displayHistory(); }
void handler_colors(char *)    { console_test();}
void handler_restart(char *)   { ESP.restart();}
void handler_detail(char *)    { s3_detail();}
void handler_temp(char *)      { Serial.printf("Internal Temperature: %.1fC\r\n",temperatureRead());}
void handler_wifi(char *)      { MyWifiScan();}
void handler_crash(char *)     { s3_crash();}
void handler_heap(char *)      { Serial.printf("Available heap: %u Bytes\r\n",ESP.getFreeHeap());}
void handler_version(char *)   { s3_version();}


console_t console[] = {
  {"help",      "Display this menu",                              0, handler_help},
  {"h",         "Display command history",                        0, handler_history},
  {"colors",    "Test the color in the AINSI console",            0, handler_colors},
  {"restart",   "Restart the ESP32-S3",                           0, handler_restart},
  {"detail",    "give detail about the ESP32-S3 uCtrl used",      0, handler_detail},
  {"temp",      "give the internal temperature of the ESP32-S3",  0, handler_temp},
  {"wifi",      "scan all wifi SSID and give the strengh of the signal",  0, handler_wifi},
  {"crash",     "List the cause of the last reset",               0, handler_crash},
  {"heap",      "Return the remaining heap",                      0, handler_heap},
  {"version",   "Return the version of ESP_Arduino used",         0, handler_version},
  {nullptr, nullptr, 0, nullptr} // Sentinelle
};

//********************************************* */

Command console_cmd = CMD_noCommand;
String Input;
String *ptr;

void setup() {
  Serial.begin(115200);
  while(!Serial){
    vTaskDelay(100/portTICK_RATE_MS);
  }
  console_displayIntro();
  ptr = &Input;

  printf("\r\n> ");
}

void loop() 
{
  vTaskDelay(10/portTICK_RATE_MS);
  int Input_char;
  char argument[10];
  int idx;

  // On récupère le caractère de l'UART
  Input_char = Serial.read();
  if (Input_char != -1) {
      console_cmd = console_getInput((uint8_t)Input_char, argument, console, &idx);

      // Si le parser a trouvé un index valide
      if (idx >= 0) {
          if (console[idx].func != NULL) {
              console[idx].func(argument);
          }
          Serial.printf("\r\n> ");
      } 
      else if (console_cmd == Error) {
          Serial.printf("Unknown command!\r\n> ");
      }
  }

}
  
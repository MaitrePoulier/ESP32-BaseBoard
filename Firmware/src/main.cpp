#include <Arduino.h>
#include <string.h>
#include "General.h"
#include "Console.h"
#include "SD.h"

#include <SPI.h>
#include "SdFat.h"


SdFat SD;
File32 myFile;

Command cmd = CMD_noCommand;
String Input;
String *ptr;

void setup() {
  Serial.begin(115200);
  while(!Serial){vTaskDelay(100);}
  
  //delay(2000);
  displayIntro();
  ptr = &Input;

  //Va au 2 diodes pour garder les DC-DC allumé quand le H7 se reprogramme
  pinMode(S3_PWR_en, OUTPUT);
  digitalWrite(S3_PWR_en, HIGH);
}

void loop() 
{
  delay(10);
  
  if (Serial.available() >0) 
  {
    cmd = getInput(ptr, Serial.read());
  }

  switch (cmd){
    case CMD_History:
      displayHistory();
    break;

    case CMD_Help:
      displayHelp();
    break;

    case CMD_SD:
      SDinit();
    break;

    case CMD_noCommand:
      //Pour l'instant on fait rien
    break;

    case CMD_Invalid:
      Serial.printf("Invalid command, try again\r\n");
    break;

    case Error:
      Serial.printf("Unknowned Errors\r\n");
    break;


    default:
      Serial.printf("Invalid CMD code\r\n");
    break;
  }

  cmd = CMD_noCommand;

}
  
#include "Console.h"
#include "Help.h"


String readString = "";
int8_t ASCII;
String History[10];
int8_t HistoryCounter = 0;

Command parser(String Input);

Command getInput(String * ptr, char c)
{
    //0 = invalid caracter or error
    //1 = ok
    Command cmd = CMD_noCommand; 
    ASCII = (int8_t)c;
    switch (ASCII)
    {
      case 13: //Si on appui sur "enter"
        //On veut que la dernière commande soit à la posion 0
        //Donc on décale tout le monde 1
        for (int i = 9; i > 0; i--){
          History[i] = History[i-1];
        }

        //On met la nouvelle valeur dans history
        History[0] = readString;

        //On imprime sur le port série un retour de ligne
        Serial.print("\r\n");
        //Debug 
        /*
        for (int i = 0; i < readString.length(); i++){
          Serial.print((int8_t)readString[i]);
          Serial.print(" ");
        }*/
        readString = "";

        *ptr = History[0];
        cmd = parser(History[0]);
        //Le seul moment où l'on ne remet pas le historycounter à FIRSTTIME
        // c'est si on a appuyé sur une flèche
        HistoryCounter = FIRST_TIME;
      break;

      case 8: //Si on fait un backspace
        Serial.print("\b \b");
        //on elève un caractère de readString
        readString.remove(readString.length()-1,1);
        *ptr = "";
        cmd = CMD_noCommand;
        //Le seul moment où l'on ne remet pas le historycounter à 0
        // c'est si on a appuyé sur une flèche
        HistoryCounter = FIRST_TIME;
      break;

      case 10:
        cmd = CMD_noCommand;
      break;

      case 27: //an arrow!!  we need to handle 3 caracters
        //Si ce n'est pas un '['
        if ((uint8_t)Serial.read()!=91){
            Serial.flush();
            cmd = Error;
            break;
        }
        //Si le 3e caractère est un 'A' c'est que c'est la flèche du haut, 'B' par en bas
        switch ((uint8_t)Serial.read())
        {
            case 65:
              //Serial.println("flèche up!!");
              if (HistoryCounter == FIRST_TIME){
                HistoryCounter = 0;
              }
              else if (HistoryCounter <= 8){
                HistoryCounter++;
              }
              Serial.printf("\x1b[2K\r");
              Serial.print(History[HistoryCounter]);
              readString = History[HistoryCounter];
              cmd = CMD_noCommand;
            break;

            case 66:
              //Serial.println("flèche Down!!");
              if (HistoryCounter == FIRST_TIME){
                HistoryCounter = 0;
              }
              else if (HistoryCounter >= 1){
                HistoryCounter--;
              }
              Serial.printf("\x1b[2K\r");
              Serial.print(History[HistoryCounter]);
              readString = History[HistoryCounter];
              cmd = CMD_noCommand;
            break;

            default:
              cmd = Error;
            break;
        }

      break;

      case 32 ... 127:  //les caractère standard
        //Add to string
        if (readString == "")
        {
            readString = c;
        }      
        else 
        {
            readString += c;
        }
        //Echo
        Serial.print(c);
        //Serial.print("\r\n");
        //Serial.print((uint32_t)c);
        //Serial.print(" ");
        
        *ptr = "";
        //Le seul moment où l'on ne remet pas le historycounter à 0
        // c'est si on a appuyé sur une flèche
        HistoryCounter = FIRST_TIME;
        cmd = CMD_noCommand;
      break;

      default:
        //Le seul moment où l'on ne remet pas le historycounter à 0
        // c'est si on a appuyé sur une flèche
        HistoryCounter = FIRST_TIME;
        cmd = Error;
      break;
    }
    return cmd;
}

void displayHistory(void){
    Serial.print("***** History: *****\r\n");
    for (int i = 0; i < 10; i++){
        if(History[i] == ""){break;}
        Serial.println(History[i]);
    }
    Serial.print("\r\n");
}

void displayIntro(){
  Serial.printf("\r\n********************************************************* \r\n");
  Serial.printf(    "   Use RealTerm in ANSI mode to support up/down arrows \r\n");
  Serial.printf(    "        Type 'help' to display all the commands \r\n");
  #ifdef PROPEL
    Serial.printf(  "                Hardware mode: Propel \r\n");
  #else
    Serial.printf(  "         Hardware mode: Adafruit Metro ESP32-S3 \r\n");
  #endif
  Serial.printf(    "********************************************************* \r\n\r\n");
}

void displayHelp(){
  Serial.print(help);
}

Command parser(String Input){
  if (Input == "help") return CMD_Help;
  else if (Input == "h") return CMD_History;
  else if (Input == "SD") return CMD_SD;
  else return CMD_Invalid;
}
#include "SD.h"

extern SdFat SD;
extern File32 myFile;

void SDinit(void){
  Serial.print("Initializing SD card...");

  //Si on utilise le Propel rev F:
  //Sinon il utilise les pins par défaut sur le Adafruit Metro ESP32-S3
  //A configurer dans General.h
  #ifdef PROPEL
    SPI.begin(CLK,MISO,MOSI,CS);
  #endif

  if (!SD.begin(CS)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("testing 1, 2, 3.");
    myFile.println("hello sd card!");
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

  // re-open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("\r\nOpening test.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}
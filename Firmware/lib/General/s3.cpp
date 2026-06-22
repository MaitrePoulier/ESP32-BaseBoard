#include "s3.h"
#include <Arduino.h>

void s3_detail(void)
{
    Serial.printf("Chip Model:    %s\r\n",ESP.getChipModel());
    Serial.printf("Chip Revision: %u\r\n",ESP.getChipRevision());
    Serial.printf("Chip Frequency:%u MHz\r\n",ESP.getCpuFreqMHz());
    Serial.printf("Fuse Mac:      ");
    Serial.print((uint32_t)(ESP.getEfuseMac() >> 32), HEX);
    Serial.println((uint32_t)ESP.getEfuseMac(), HEX);
}


void s3_version(void)
{
      Serial.printf("PlatformIO Espressif32 version: %s\r\n", PIO_PLATFORM_VERSION);
      Serial.printf("ESP32 Arduino version: %s\r\n", ESP_ARDUINO_VERSION_STR);
      //Appel direct de la fonction système (C/C++ standard)
      Serial.printf("ESP-IDF version: %s\r\n", esp_get_idf_version());
}

void s3_crash(void)
{
  Serial.print("Reset reason: ");

  switch(esp_reset_reason()) {
    case ESP_RST_UNKNOWN: 
      Serial.println("Unknown"); 
      break;
    case ESP_RST_POWERON: 
      Serial.println("Power-on"); 
      break;
    case ESP_RST_EXT: 
      Serial.println("External reset"); 
      break;
    case ESP_RST_SW: 
      Serial.println("Software reset"); 
      break;
    case ESP_RST_PANIC: 
      Serial.println("Exception/panic"); 
      break;
    case ESP_RST_INT_WDT: 
      Serial.println("Interrupt watchdog"); 
      break;
    case ESP_RST_TASK_WDT: 
      Serial.println("Task watchdog"); 
      break;
    case ESP_RST_WDT: 
      Serial.println("Other watchdog"); 
      break;
    case ESP_RST_DEEPSLEEP: 
      Serial.println("Wake from deep sleep"); 
      break;
    case ESP_RST_BROWNOUT: 
      Serial.println("Brownout reset"); 
      break;
    case ESP_RST_SDIO: 
      Serial.println("SDIO reset"); 
      break;
    default: 
      Serial.println("Unknown reason"); 
      break;
  }
}
#include <Arduino.h>
#include <string.h>
#include "Config.h"
#include "Console.h"
#include <WiFi.h>
#include "MyWifi.h"
#include "s3.h"
#include "Cellular.h"
#include "Batt.h"
#include "adc_cali_schemes.h"

// LVGL specific
#include "lvgl.h"
#define  LV_CONF_INCLUDE_SIMPLE
#include "Lvgl_code.h"
#include "demos/lv_demos.h"


// Hardware-specific library for the TFT screen
#include <SPI.h>
#include <TFT_eSPI.h> 
TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

static lv_disp_t * disp;
static lv_indev_t * indev; // LVGL input device handle
static lv_color_t *buf1 = nullptr;
static lv_color_t *buf2 = nullptr;

//#define LV_USE_DRAW_SW 1
//#define LV_DRAW_SW_COMPLEX 1



/********************* Console **************************************/
// Handlers simples
extern console_t console[13];
void handler_help(char *)      { console_displayHelp(console); }
void handler_history(char *)   { console_displayHistory(); }
void handler_colors(char *)    { console_test();}
void handler_restart(char *)   { ESP.restart();}
void handler_detail(char *)    { ESP32_detail();}
void handler_temp(char *)      { Serial.printf("Internal Temperature: %.1fC\r\n",temperatureRead());}
//void handler_batt(char *)      { Serial.printf("Battery Voltage: %.1fV\r\n",battRead());}
void handler_wifi(char *)      { MyWifiScan();}
void handler_crash(char *)     { ESP32_crash();}
void handler_heap(char *)      { Serial.printf("Available heap: %u Bytes\r\n",ESP.getFreeHeap());}
void handler_version(char *)   { ESP32_version();}
void handler_lvgVersion(char *)  { Serial.printf("LVGL version %d.%d.%d\r\n",lv_version_major(), lv_version_minor(), lv_version_patch());}
void handler_touch(char *)     { touch_calibrate();}


console_t console[] = {
  {"help",       "Display this menu",                              0, handler_help},
  {"h",          "Display command history",                        0, handler_history},
  {"colors",     "Test the color in the AINSI console",            0, handler_colors},
  {"restart",    "Restart the ESP32-WROOM-32E-N8R2",               0, handler_restart},
  {"detail",     "give detail about the ESP32-S3 uCtrl used",      0, handler_detail},
  {"temp",       "give the internal temperature of the ESP32",     0, handler_temp},
  //{"batt",      "give the voltage of the battery",               0, handler_batt},
  {"wifi",       "scan all wifi SSID and give the strengh of the signal",  0, handler_wifi},
  {"crash",      "List the cause of the last reset",               0, handler_crash},
  {"heap",       "Return the remaining heap",                      0, handler_heap},
  {"version",    "Return the version of ESP_Arduino used",         0, handler_version},
  {"lvglVersion","Return the version of the LVGL library",         0, handler_lvgVersion},
  {"calTouch",   "Calibrate the touch screen",                     0, handler_touch},
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

  //*************************
  // Set up the display
  ScreenInit();

  //Use the touch calibration routine to get the correct values for your screen
  uint16_t calData[5] = { 288, 3622, 220, 3652, 5 }; 
  tft.setTouch((uint16_t *)calData);

  //*********************************
  // Initialize LVGL
  // 1. Calculate buffer size in pixels and bytes
  uint32_t buf_pixel_count = SCREEN_WIDTH * (SCREEN_HEIGHT / 10);
  uint32_t buf_byte_size = buf_pixel_count * sizeof(lv_color_t);

  // 2. Allocate dynamically from fast internal DMA memory
  buf1 = (lv_color_t *)heap_caps_malloc(buf_byte_size, MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);
  buf2 = (lv_color_t *)heap_caps_malloc(buf_byte_size, MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);


  // 3. Fallback check to ensure allocation succeeded
  if (buf1 == nullptr || buf2 == nullptr) {
      Serial.println("Critical Error: DMA Buffer allocation failed!");
      while(1) { vTaskDelay(1000 / portTICK_RATE_MS); }
  }

  lv_init();

  lv_tick_set_cb(my_tick);
  
  // Create display 
  disp = lv_display_create(SCREEN_WIDTH, SCREEN_HEIGHT); 
  lv_display_set_flush_cb(disp, my_flush_cb);
  // Change sizeof(buf1) to buf_byte_size
  lv_display_set_buffers(disp, buf1, buf2, buf_byte_size, LV_DISPLAY_RENDER_MODE_PARTIAL);

  // Setup Touch Input
  indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(indev, my_touchpad_read);

  //create_test_ui();
  lv_demo_benchmark(); 

  //**************************

  // Set 12-bit resolution (0-4095)
  analogSetWidth(12);
  // Set ADC attenuation to 6 dB (optimal for ~1.25V - 1.85V input)
  analogSetPinAttenuation(39, ADC_6db);

}

void loop() 
{
  /*char buffer[10];
  float VBatt, ChargeBatt;
  VBatt = (float)(analogReadMilliVolts(BattPin) * 2.0 * CALIBRATION_FACTOR / 1000.0);
  ChargeBatt = getBatteryPercentage(VBatt);
  sprintf(buffer, "Batt: %.2fV - %.0f%%",VBatt,ChargeBatt);
  game(buffer);*/

  // Let the GUI do its work
  lv_timer_handler(); 

  //vTaskDelay(5/portTICK_RATE_MS);
  
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
  
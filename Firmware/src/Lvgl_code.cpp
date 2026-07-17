#include "Lvgl_code.h"
#include <TFT_eSPI.h> 

extern TFT_eSPI tft;

void my_flush_cb(lv_display_t * disp, const lv_area_t * area, uint8_t * px_map) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    
    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushPixelsDMA((uint16_t *)px_map, w * h);
    tft.dmaWait();
    //tft.pushColors((uint16_t *)px_map, w * h, true);
    tft.endWrite();
    
    lv_display_flush_ready(disp);
}
/*
void my_flush_cb(lv_display_t *display, const lv_area_t *area, unsigned char *data)
{

  uint32_t w = lv_area_get_width(area);
  uint32_t h = lv_area_get_height(area);
  tft.startWrite();
  tft.pushImageDMA(area->x1, area->y1, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1, (uint16_t *)data);
  tft.endWrite();
  lv_display_flush_ready(display); /* tell lvgl that flushing is done
}*/

// Touchpad Read Callback
void my_touchpad_read(lv_indev_t * indev, lv_indev_data_t * data) {
    uint16_t touchX = 0, touchY = 0;
    
    // Check hardware reading
    bool touched = tft.getTouch(&touchX, &touchY);
    
    if(!touched) {
        data->state = LV_INDEV_STATE_RELEASED;
    } else {
        // Debugging prints - Check your Serial Monitor!
        /*Serial.print("Touch detected! X: ");
        Serial.print(touchX);
        Serial.print(" Y: ");
        Serial.println(touchY);*/

        data->state = LV_INDEV_STATE_PRESSED;
        data->point.x = touchX;
        data->point.y = touchY;
    }
}


// Simple test button event callback
static void btn_event_cb(lv_event_t * e) {
    // Added explicit typecast to fix the C++ conversion error
    lv_obj_t * btn = (lv_obj_t *)lv_event_get_target(e);
    lv_obj_t * label = lv_obj_get_child(btn, 0);
    lv_label_set_text(label, "Clicked!");
}

// Your UI Definition Function
void create_test_ui(void) {
    // Create an Interactive Button to test touch functionality
    lv_obj_t * btn = lv_btn_create(lv_scr_act());
    lv_obj_align(btn, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t * label = lv_label_create(btn);
    lv_label_set_text(label, "Touch Me");
    lv_obj_center(label);
}

uint32_t my_tick(void)
{
  return millis();
}

void ScreenInit(void){
  //Set up the display
  tft.init();
  tft.initDMA(); 
  //ADD THIS LINE TO FIX DMA COLORS
  //Maybe I will have color problem when I use something else than LVGL routine
  tft.setSwapBytes(true); 
  
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(0, 0);
}

//From tft_eSPI/examples/Generic/touch_calibrate.ino 
void touch_calibrate()
{
  uint16_t calData[5];
  uint8_t calDataOK = 0;

  // Calibrate
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(20, 0);
  tft.setTextFont(2);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  tft.println("Touch corners as indicated");

  tft.setTextFont(1);
  tft.println();

  tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);

  Serial.println(); Serial.println();
  Serial.println("// Use this calibration code in setup():");
  Serial.print("  uint16_t calData[5] = ");
  Serial.print("{ ");

  for (uint8_t i = 0; i < 5; i++)
  {
    Serial.print(calData[i]);
    if (i < 4) Serial.print(", ");
  }

  Serial.println(" };");
  Serial.print("  tft.setTouch(calData);");
  Serial.println(); Serial.println();

  tft.fillScreen(TFT_BLACK);
  
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.println("Calibration complete!");
  tft.println("Calibration code sent to Serial port.");

  delay(4000);
}
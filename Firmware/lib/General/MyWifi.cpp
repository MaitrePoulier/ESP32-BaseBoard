#include <MyWifi.h>

//Create the wifi task
void MyWifiScan(void)
{
  xTaskCreatePinnedToCore(MyWifiScanTask, 
                          "Wifi task",
                          4096,     //WiFi.scanNetworks() is very big, need space
                          NULL,
                          10,
                          NULL,
                          1         // Core ID (1 = App Core, 0 = Protocol Core
                        ); 
}


void MyWifiScanTask(void * pvParameters)
{

  Serial.println("Scanning WiFi networks...");

  WiFi.mode(WIFI_STA);   // Set as station
  WiFi.disconnect();     // Make sure we're not connected
  vTaskDelay(100/portTICK_PERIOD_MS);

  int n = WiFi.scanNetworks();

  if (n == 0) {
    Serial.printf("No networks found.\r\n");
  } 
  else {
    Serial.printf("%d networks found:\r\n", n);

    for (int i = 0; i < n; ++i) {
      Serial.printf("%d: %s | RSSI: %d dBm | %s\r\n",
        i + 1,
        WiFi.SSID(i).c_str(),
        WiFi.RSSI(i),
        (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? "Open" : "Secured");
      
        vTaskDelay(10/portTICK_PERIOD_MS);
    }
  }

  Serial.printf("\r\nScan complete.\r\n> ");
  vTaskDelete(NULL);
}
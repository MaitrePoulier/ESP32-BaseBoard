#include "Arduino.h"
#include "H7serial.h"
#include "General.h"

extern HardwareSerial H7Serial;

void sendTimeToH7()
{
  uint32_t now = millis() / 1000;

  char buffer[40];
  int len = snprintf(buffer, sizeof(buffer),
                     "$T,%lu#",
                     now);

  size_t available = H7Serial.availableForWrite();

  if (available >= len)
  {
    H7Serial.write((uint8_t*)buffer, len);
  }
  else
  {
    //Serial.println("H7 UART not ready (CTS blocking or buffer full)");
  }
}

void H7serial_init()
{
  H7Serial.setPins(UART_RX, UART_TX, UART_CTS, UART_RTS);           // TX, RX, RTS, CTS
  H7Serial.begin(115200, SERIAL_8N1, UART_RX, UART_TX); // RX, TX
  H7Serial.setHwFlowCtrlMode(UART_HW_FLOWCTRL_CTS_RTS, 64);
  //Serial.println("UART to H7 initialized\r\n");

  // 🚨 disable flow control completely
  //H7Serial.setHwFlowCtrlMode(UART_HW_FLOWCTRL_DISABLE, 0);
}



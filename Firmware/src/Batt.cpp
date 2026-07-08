#include "Batt.h"

float getBatteryPercentage(float voltage) {
  // Handle upper boundary
  if (voltage >= socTable[0].voltage) return 100.0;
  // Handle lower boundary
  if (voltage <= socTable[TABLE_SIZE - 1].voltage) return 0.0;

  // Search for the voltage segment inside the table
  for (int i = 0; i < TABLE_SIZE - 1; i++) {
    if (voltage <= socTable[i].voltage && voltage >= socTable[i+1].voltage) {
      // Linear interpolation formula between point i and i+1
      float v_high = socTable[i].voltage;
      float v_low  = socTable[i+1].voltage;
      float p_high = socTable[i].percentage;
      float p_low  = socTable[i+1].percentage;

      return p_low + ((voltage - v_low) / (v_high - v_low)) * (p_high - p_low);
    }
  }
  return 0.0; 
}


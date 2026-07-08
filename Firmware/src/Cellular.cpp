#include "Cellular.h"
#include "General.h"

#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library

//Current grid
uint8_t grid[GRIDX][GRIDY];
//The new grid for the next generation
uint8_t newgrid[GRIDX][GRIDY];

//The generation or iteration we now are
uint16_t gen = 0;

#define InitialColor 0x1111
uint16_t actualColor;


void drawGrid(void);
void initGridText(const char* );
void computeCA();
int getNumberOfNeighbors(int, int);



void ScreenInit(void){
  //Set up the display
  tft.init();
  tft.initDMA(); 
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(0, 0);
}

void game(const char* text){
    if (gen == 0)
    {
        initGridText(text);
        actualColor = InitialColor;
        tft.fillScreen(TFT_BLACK);

        //Force-render Generation 0 directly to the hardware 
        for (int16_t x = 1; x < GRIDX - 1; x++) {
            for (int16_t y = 1; y < GRIDY - 1; y++) {
                if (grid[x][y] == 1) {
                    tft.fillRect(CELLXY * x, CELLXY * y, CELLXY, CELLXY, TFT_WHITE); //(0xFFFF));
                }
                // CRITICAL: Set newgrid equal to grid so drawGrid doesn't see a "change" 
                // and try to instantly wipe it out on the next frame.
                newgrid[x][y] = grid[x][y]; 
            }
        }
        gen++;
    }
    else if(gen < 100)
    {    
        delay(1);
        gen++;
    }
    else
    {
        //Compute generations
        computeCA();
        drawGrid();
        for (int16_t x = 1; x < GRIDX-1; x++) 
        {
            for (int16_t y = 1; y < GRIDY-1; y++) 
            {
                grid[x][y] = newgrid[x][y];
            }
        }
        gen++;
        if (gen >= NUMGEN) gen = 0;
    }
}

//Draws the grid on the display
void drawGrid(void) {
  uint16_t color = TFT_WHITE;

  for (int16_t x = 1; x < GRIDX - 1; x++) {
    for (int16_t y = 1; y < GRIDY - 1; y++) {
      // Only draw if the cell state changed from the last generation
      if ((grid[x][y]) != (newgrid[x][y])) {
        // If the current cell is alive, color it. If dead (0), paint it black.
        if (newgrid[x][y] == 1) color = actualColor; //random(0xFFFF); 
        else color = TFT_BLACK;
        tft.fillRect(CELLXY * x, CELLXY * y, CELLXY, CELLXY, color);
      }
    }
  }
  actualColor = actualColor + 100;
}

//Initialise Grid
void initGridText(const char* message) {
  // 1. Fully clear both tracking arrays
  for (int16_t x = 0; x < GRIDX; x++) {
    for (int16_t y = 0; y < GRIDY; y++) {
      grid[x][y] = 0;
      newgrid[x][y] = 0;
    }
  }

  // 2. Setup boundaries inside the padding perimeter
  int16_t spriteW = GRIDX - 2;
  int16_t spriteY = GRIDY - 2;
  
  TFT_eSprite spr = TFT_eSprite(&tft);
  spr.setColorDepth(1); 
  spr.createSprite(spriteW, spriteY);
  
  spr.fillSprite(0);
  spr.setTextColor(1, 0); 
  spr.setTextDatum(MC_DATUM); 
  
  // Font 2 requires at least 16 pixels of height. 
  // If your screen grid height is tiny, change the 2 below to a 1.
  spr.drawString(message, spriteW / 2, spriteY / 2, 2); 

  // 3. Map memory layout securely into CA framework
  for (int16_t sx = 0; sx < spriteW; sx++) {
    for (int16_t sy = 0; sy < spriteY; sy++) {
      // FIX: Any non-zero response indicates active font typography mask
      if (spr.readPixel(sx, sy) != 0) { 
        grid[sx + 1][sy + 1] = 1; 
      }
    }
  }

  spr.deleteSprite();
}

//Compute the CA. Basically everything related to CA starts here
void computeCA() {
  for (int16_t x = 1; x < GRIDX; x++) {
    for (int16_t y = 1; y < GRIDY; y++) {
      int neighbors = getNumberOfNeighbors(x, y);
      if (grid[x][y] == 1 && (neighbors == 2 || neighbors == 3 ))
      {
        newgrid[x][y] = 1;
      }
      else if (grid[x][y] == 1)  newgrid[x][y] = 0;
      if (grid[x][y] == 0 && (neighbors == 3))
      {
        newgrid[x][y] = 1;
      }
      else if (grid[x][y] == 0) newgrid[x][y] = 0;
    }
  }
}

// Check the Moore neighbourhood
int getNumberOfNeighbors(int x, int y) {
  return grid[x - 1][y] + grid[x - 1][y - 1] + grid[x][y - 1] + grid[x + 1][y - 1] + grid[x + 1][y] + grid[x + 1][y + 1] + grid[x][y + 1] + grid[x - 1][y + 1];
}

/*
   The MIT License (MIT)

   Copyright (c) 2016 RuntimeProjects.com

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/


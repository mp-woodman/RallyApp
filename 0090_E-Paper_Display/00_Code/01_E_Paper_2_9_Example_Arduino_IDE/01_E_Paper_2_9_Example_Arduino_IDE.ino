// GxEPD_MinimumExample by Jean-Marc Zingg

#include <GxEPD.h>

// select the display class to use, only one, copy from GxEPD_Example
//#include <GxGDEH0154D67/GxGDEH0154D67.h>  // 1.54" b/w
#include <GxGDEH029A1/GxGDEH029A1.h>      // 2.9" b/w

#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>

//Fonts
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>

// constructor for ESP32, copy from GxEPD_Example else
GxIO_Class io(SPI, /*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16); // arbitrary selection of 17, 16
GxEPD_Class display(io, /*RST=*/ 16, /*BUSY=*/ 4); // arbitrary selection of (16), 4

void setup()
{
  Serial.begin(115200);
  Serial.println("Testprogramm Epaper");
  display.init();
  display.eraseDisplay();
}

void loop() 
{
  showFont("FreeMonoBold12pt7b", &FreeMonoBold12pt7b);
};

void showFont(const char name[], const GFXfont* f)
{
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);
  display.setFont(f);
  display.setCursor(0, 0);
  display.println();
  display.print("Auto: ");
  display.println(1);
  display.println();
  display.println("Messung:");
  display.println(24);
  display.println();
  display.println("Dif:");
  display.println("+0.56 s");
  display.println();
  display.println("um:");
  display.println("12:23 Uhr\n42,12 s"); 
  display.update();
  delay(5000);
}

/*****************************************************************
MicroView_Analog_Clock.ino
SFE_LSM9DS0 Library Simple Example Code
Jim Lindblom @ SparkFun Electronics
Original Creation Date: February 28, 2015
https://github.com/sparkfun/MicroView/tree/master/Libraries

Turn your MicroView into an analog clock!

This sketch requires the Arduino time library. Get it from
here: http://playground.arduino.cc/Code/Time

If you are using Platformio, install the Time library with
   pio lib install "Time"

Development environment specifics:
	IDE: Arduino 1.6.0
	Hardware Platform: MicroView

This code is beerware. If you see me (or any other SparkFun 
employee) at the local, and you've found our code helpful, please 
buy us a round!

Distributed as-is; no warranty is given.
*****************************************************************/
#include <MicroView.h>
#include <Time.h>

// Define how big the clock is. Don't make it larger than 23
// This is the radius of the clock:
#define CLOCK_SIZE 23

const uint8_t maxW = uView.getLCDWidth();
const uint8_t midW = maxW/2;
const uint8_t maxH = uView.getLCDHeight();
const uint8_t midH = maxH/2;

// Draw the clock face. That includes the circle outline and
// the 12, 3, 6, and 9 text.
void drawFace()
{
  uView.setFontType(0); // set font type 0 (Smallest)

  uint8_t fontW = uView.getFontWidth();
  uint8_t fontH = uView.getFontHeight();

  //uView.setCursor(27, 0); // points cursor to x=27 y=0
  uView.setCursor(midW-fontW-1, midH-CLOCK_SIZE+1);
  uView.print(12);  // Print the "12"
  uView.setCursor(midW-(fontW/2)-1, midH+CLOCK_SIZE-fontH-1);
  uView.print(6);  // Print the "6"
  uView.setCursor(midW-CLOCK_SIZE+1, midH-fontH/2);
  uView.print(9);  // Print the "9"
  uView.setCursor(midW+CLOCK_SIZE-fontW-2, midH-fontH/2);
  uView.print(3);  // Print the "3"
  uView.circle(midW-1, midH-1, CLOCK_SIZE);

  //Draw the clock
  uView.display();
}

void setup()
{
  int year, month, day, hour, minute, second;

  // Random trick on Arduino from
  // https://programmingelectronics.com/using-random-numbers-with-arduino/
  randomSeed(analogRead(A0));
  hour = random(12);
  minute = random(60);
  second = random(60);
  day = random(1,32);
  month = random(1,13);
  year = random(2015,2066);
  // Set the time in the time library:
  setTime(hour, minute, second, day, month, year);

  uView.begin();    // set up the MicroView
  uView.clear(PAGE);// erase hardware memory inside the OLED
  uView.display();  // display the content in the buffer

  // Draw clock face (circle outline & text):
  drawFace();
}

void drawTime()
{
  static boolean firstDraw = false;
  static unsigned long mSec = millis() + 1000;
  static float degresshour, degressmin, degresssec,
               hourx, houry, minx, miny, secx, secy;
  // If mSec
  if (mSec != (unsigned long)second()) 
  {
    // First time draw requires extra line to set up XOR's:
    if (firstDraw) 
    {
      uView.line(midW, midH, 32 + hourx, 24 + houry, WHITE, XOR);
      uView.line(midW, midH, 32 + minx, 24 + miny, WHITE, XOR);
      uView.line(midW, midH, 32 + secx, 24 + secy, WHITE, XOR);
    }
    // Calculate hour hand degrees:
    degresshour = (((hour() * 360) / 12) + 270) * (PI / 180);
    // Calculate minute hand degrees:
    degressmin = (((minute() * 360) / 60) + 270) * (PI / 180);
    // Calculate second hand degrees:
    degresssec = (((second() * 360) / 60) + 270) * (PI / 180);

    // Calculate x,y coordinates of hour hand:
    hourx = cos(degresshour) * (CLOCK_SIZE / 2.5);
    houry = sin(degresshour) * (CLOCK_SIZE / 2.5);
    // Calculate x,y coordinates of minute hand:
    minx = cos(degressmin) * (CLOCK_SIZE / 1.4);
    miny = sin(degressmin) * (CLOCK_SIZE / 1.4);
    // Calculate x,y coordinates of second hand:
    secx = cos(degresssec) * (CLOCK_SIZE / 1.1);
    secy = sin(degresssec) * (CLOCK_SIZE / 1.1);

    // Draw hands with the line function:
    uView.line(midW, midH, midW+hourx, midH+houry, WHITE, XOR);
    uView.line(midW, midH, midW+minx, midH+miny, WHITE, XOR);
    uView.line(midW, midH, midW+secx, midH+secy, WHITE, XOR);
    
    // Set firstDraw flag to true, so we don't do it again.
    firstDraw = true;
    
    // Actually draw the hands with the display() function.
    uView.display();
  }
}

void loop()
{
  drawTime();
}

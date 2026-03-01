/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 modified 7 Nov 2016
 by Arturo Guadalupi

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystalHelloWorld

*/

// include the library code:
#include <LiquidCrystal.h>
#include <PushButton.h>

//définition de l'écran lcd 16x02
const int rs = 8, en = 9, d4 = 17, d5 = 16, d6 = 15, d7 = 14;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//définition des boutons
PushButton BTNmin(2);
PushButton BTNmode(3);
PushButton BTNmax(4);
PushButton BTNup(11);
PushButton BTNdown(12);

void setup() {
  Serial.begin(115200);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");

  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(11, INPUT);
  pinMode(12, INPUT);

  //définition des boutons
  BTNmin.setActiveLogic(LOW);
  BTNmode.setActiveLogic(HIGH);
  BTNmax.setActiveLogic(HIGH);
  BTNup.setActiveLogic(LOW);
  BTNdown.setActiveLogic(LOW);

  BTNmin.disableDoubleClick();
  BTNmode.disableDoubleClick();
  BTNmax.disableDoubleClick();
  BTNup.disableDoubleClick();
  BTNdown.disableDoubleClick();

  BTNmin.setHoldTime(500);
  BTNmode.setHoldTime(5000);
  BTNmax.setHoldTime(500);
  BTNup.setHoldTime(500);
  BTNdown.setHoldTime(500);

  BTNmode.setDebounceTime(40);

  BTNmin.update();
  BTNmode.update();
  BTNmax.update();
  BTNup.update();
  BTNdown.update();
}

void loop() {

  BTNmin.update();
  BTNmode.update();
  BTNmax.update();
  BTNup.update();
  BTNdown.update();

  lcd.setCursor(0,0);
  if (BTNmode.isClicked()) {
    lcd.print("mode");
    delay(1000);
    lcd.clear();
  }
  if (BTNmax.isClicked()) {
    lcd.print("max");
    delay(1000);
    lcd.clear();
  }
  if(BTNmode.isActive()) {
    lcd.setCursor(0,1);
    lcd.print("m");
  }
}

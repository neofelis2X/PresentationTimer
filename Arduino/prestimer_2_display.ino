// here go the display related functions

/* functions:

display.setBrightness(7, false);  // Turn off

uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
display.setSegments(data);

data[0] = display.encodeDigit(0);
data[1] = display.encodeDigit(1);
data[2] = display.encodeDigit(2);
data[3] = display.encodeDigit(3);
display.setSegments(data);

display.setSegments(data+2, 2, 2);

display.clear();

display.showNumberDec(-5, false, 3, 0); // Expect: _-5_
display.showNumberHexEx(0xf1af);        // Expect: f1Af

*/

void refreshSetTime(uint8_t m, uint8_t s, uint8_t blinkPos) {
  // blinkPos defines the position of the blinking fields

  static unsigned long lastDispBlinkChange;
  static bool blinkDispState = 0;

  if (millis() - lastDispBlinkChange > 1000) {
    blinkDispState = !blinkDispState;
    lastDispBlinkChange = millis();
  }

  if (blinkDispState == HIGH) {
    display.showNumberDec(m, 0b01000000, false, 2, 0);  // Expect: NN:00
    display.showNumberDec(s, 0b01000000, false, 2, 2);  // Expect: NN:00
                                                        // refresh time on display
  } else {
    display.setSegments(0b01000000, blinkPos);
    display.setSegments(0b01000000, blinkPos++);
    // turn minutes of display off
  }
  //minutes are blinking
}

void refreshCounting(unsigned long value) {
  //input is in milliseconds
  // divide 1000 to get seconds, divide 60 to get minutes
  byte minT = value / 1000 / 60;
  // divide 1000 to get seconds, modulus of 60 gives remaining seconds
  byte secT = value / 1000 % 60;
  display.clear();
  display.showNumberDec(minT, 0b01000000, false, 2, 0);  // Expect: NN:00
  display.showNumberDec(secT, 0b01000000, false, 2, 2);  // Expect: 00:NN
}

void refreshMenu() {
  switch (CurrItem) {
    case menuItem::RESET:
      refreshText(8, 9, 4, 10);
      break;
    case menuItem::UP:
      refreshText(12, 12, 11, 7);
      break;
    case menuItem::DOWN:
      refreshText(3, 6, 11, 5);
      break;
    case menuItem::BACK:
      refreshText(1, 0, 2, 2);
      break;
  }
}

void refreshText(uint8_t i, uint8_t j, uint8_t k, uint8_t l) {
  display.clear();
  display.setSegments(Letter[i], 0);
  display.setSegments(Letter[j], 1);
  display.setSegments(Letter[k], 2);
  display.setSegments(Letter[l], 3);
}
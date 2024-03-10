//here go elements of the state machine

void stateMenu() {
  debugln(F("Entered Menu"));
  static byte itemNum = 0;
  itemNum = checkRotaryPosition(itemNum, 3, 0);
  menuItemToEnum(itemNum);
  refreshMenu();

  if (ButtonClickState) {
    resetButtonStates();
    transExitMenu();
  }
  // 0 RESET (to zero or to timeDownSet)
  // 1 CHANGE TO COUNTDOWN
  // 2 CHANGE TO COUNTUP
  // 3 GO BACK
}

void statePaused() {
  debugln(F("Counting is Paused"));

  refreshCounting(timeReal);

  if (ButtonClickState) {
    resetButtonStates();
    transEndPaus();
  }

  if (ButtonLongState) {
    resetButtonStates();
    transOpenMenu();
  }
}

void stateCountDown() {
  debugln(F("Running Countdown"));

  timeReal = timeDownNow + timeStart - millis();
  refreshCounting(timeReal);

  if (ButtonClickState) {
    resetButtonStates();
    transStartPaus();
  }

  if (ButtonLongState) {
    resetButtonStates();
    transOpenMenu();
  }
}

void stateCountUp() {
  debugln(F("Running CountUp"));

  timeReal = timeReal + millis() - timeStart;
  refreshCounting(timeReal);

  if (ButtonClickState) {
    resetButtonStates();
    transStartPaus();
  }

  if (ButtonLongState) {
    resetButtonStates();
    transOpenMenu();
  }
}


/*
void menue() {
  byte menueOption = 1;  // 1: UP, 2: DOWN, 3:BACC
  display.clear();
  display.setSegments(DOWN);
  Serial.println("Menue aufgerufen");

  unsigned long menueIdle = millis();

  int pos = 0;
  while (millis() - menueIdle < 60000) {
    encoder->tick();
    int newPos = encoder->getPosition();
    Serial.print(F("getPosition: "));
    Serial.println(newPos);

    if (pos != newPos) {
      pos = newPos;
      Serial.print(F("Drehrichtung (-1 oder 1): "));
      Serial.println(int(encoder->getDirection()));
      menueOption = menueOption + int(encoder->getDirection());
      if (menueOption == 0) {
        menueOption = 3;
      } else if (menueOption == 4) {
        menueOption = 1;
      }

      switch (menueOption) {
        case 1:  //Menueoption DOWN runterzaehlen
          Serial.println(F("Menueoption 1 - DOWN"));
          display.clear();
          display.setSegments(DOWN);

          break;
        case 2:  //Menueoption UP raufzaehlen
          Serial.println(F("Menueoption 2 - UP"));
          display.clear();
          display.setSegments(UP);

          break;
        case 3:  //Menueoption BACC zurueckgehen
          Serial.println(F("Menueoption 3 - BACK"));
          display.clear();
          display.setSegments(BACC);
          break;
      }
    }

    if (debounce()) {
      switch (menueOption) {
        case 1:
          Serial.println(F("Ausgewaehlt - DOWN"));
          countdown();
          break;

        case 2:
          Serial.println(F("Ausgewaehlt - UP"));
          countup();
          break;

        case 3:
          Serial.println(F("Ausgewaehlt - BACK"));
          display.showNumberDecEx(0, 0b01000000, false, 0, 0);
          display.setSegments(HYPHEN);
          break;
      }
      break;
    }
  }
  Serial.println(F("Menue timed out or ended."));
}

byte menueEnd() {
  byte menueOption = 1;  // 1: STOP, 2:BACC
  display.clear();
  display.setSegments(STOP);
  Serial.println("MenueEnd aufgerufen");

  unsigned long menueIdle = millis();

  int pos = 0;
  while (millis() - menueIdle < 10000) {
    encoder->tick();
    int newPos = encoder->getPosition();
    Serial.print(F("getPosition: "));
    Serial.println(newPos);

    if (pos != newPos) {
      pos = newPos;
      Serial.print(F("Drehrichtung (-1 oder 1): "));
      Serial.println(int(encoder->getDirection()));
      menueOption = menueOption + int(encoder->getDirection());
      if (menueOption == 0) {
        menueOption = 2;
      } else if (menueOption == 3) {
        menueOption = 1;
      }

      switch (menueOption) {
        case 1:  //Menueoption DOWN runterzaehlen
          Serial.println(F("Menueoption 1 - STOP"));
          display.clear();
          display.setSegments(STOP);
          break;
        case 2:  //Menueoption BACC zurueckgehen
          Serial.println(F("Menueoption 2 - BACK"));
          display.clear();
          display.setSegments(BACC);
          break;
      }
    }

    if (debounce()) {
      switch (menueOption) {
        case 1:
          Serial.println(F("Ausgewaehlt - STOP"));
          countdown();
          return 1;

        case 2:
          Serial.println(F("Ausgewaehlt - BACK"));
          return 0;
      }
    }
  }
  Serial.println(F("MenueEnd timed out (>10s) or ended unusual."));
  return 0;
}
*/
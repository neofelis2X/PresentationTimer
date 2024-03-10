// all the transitions between the states come in this group

void transStartPaus() {
  //Reset the button variables
  resetButtonStates();
  LastState = CurrState;
  CurrState = timerState::COUNTPAUSED;
}

void transEndPaus() {
  resetButtonStates();
  CurrState = LastState;
  timeStart = millis();
}

void transOpenMenu() {
  resetButtonStates();
  LastState = CurrState;
  CurrState = timerState::MENU;
}

void transExitMenu() {
  resetButtonStates();
  switch (CurrItem) {
    // RESET (to zero or to timeDownSet)
    case menuItem::RESET:
      timeReal = 0;
      if (LastState == timerState::COUNTDOWN) {
        timeDownNow = timeDownSet;
      }
      CurrState = timerState::COUNTPAUSED;
      break;

    // CHANGE TO COUNTUP
    case menuItem::UP:
      timeReal = 0;
      CurrState = timerState::COUNTPAUSED;
      // the PAUSED state starts the next counting based on the last state
      LastState = timerState::COUNTUP;
      break;

      // CHANGE TO COUNTDOWN
    case menuItem::DOWN:
      // setting a new time before the countdown can start
      timeDownSet = setTime();
      CurrState = timerState::COUNTPAUSED;
      // the PAUSED state starts the next counting based on the last state
      LastState = timerState::COUNTDOWN;
      break;

    //GO BACK
    case menuItem::BACK:
      CurrState = LastState;
      timeStart = millis();  // resetting the startTime to right now and start counting
      // question: do we jump back into running mode or into paused mode?
      // Alternative: CurrState = timerState::COUNTPAUSED; // this jumps back into paused mode
      break;

    default: break;
  }
}

unsigned long setTime() {
  debugln(F("Setting the Time"));
  uint8_t minutes = 0;
  uint8_t seconds = 0;

  resetButtonStates();
  while (!ButtonClickState) {
    minutes = checkRotaryPosition(minutes, 59, 0);
    refreshSetTime(minutes, seconds, 0);
  }

  resetButtonStates();
  while (!ButtonClickState) {
    seconds = checkRotaryPosition(seconds, 59, 0);
    refreshSetTime(minutes, seconds, 2);
  }
  //convert seconds to milliseconds and add to the already set minutes
  unsigned long timeMillis = 0;
  timeMillis = minutes * 60 * 1000 + seconds * 1000;

  return timeMillis;
}

uint8_t checkRotaryPosition(uint8_t relativPos, uint8_t maxLimit, uint8_t minLimit) {
  static int absolutPos;
  encoder->tick();
  int absolutPosNew = encoder->getPosition();
  if (absolutPos != absolutPosNew) {
    absolutPos = absolutPosNew;
    relativPos += int(encoder->getDirection());
  }
  if (relativPos > maxLimit) relativPos = minLimit;
  else if (relativPos < minLimit) relativPos = maxLimit;
  return relativPos;
}

void menuItemToEnum(uint8_t item) {
  switch (item) {
    case 0:
      CurrItem = menuItem::RESET;
      break;
    case 1:
      CurrItem = menuItem::UP;
      break;
    case 2:
      CurrItem = menuItem::DOWN;
      break;
    case 3:
      CurrItem = menuItem::BACK;
      break;
    default: break;
  }
}

void resetButtonStates() {
  // reset both button states to make sure no old states are kept too long
  ButtonClickState = 0;
  ButtonLongState = 0;
}

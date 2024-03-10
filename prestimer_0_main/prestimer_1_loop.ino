// only the main loop is in this section

void loop() {

  switch (CurrState) {

    case timerState::MENU:
      stateMenu();
      break;

    case timerState::COUNTPAUSED:
      statePaused();
      break;

    case timerState::COUNTUP:
      stateCountUp();
      break;

    case timerState::COUNTDOWN:
      stateCountDown();
      break;
  }
}
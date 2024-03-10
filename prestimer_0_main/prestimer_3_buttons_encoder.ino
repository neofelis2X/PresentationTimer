// here goes everthing that concerns the pushbutton and the rotary encoder

void buttonClicked() {
  debugln("Button click registered.");
  ButtonClickState = 1;
}

void buttonLongPressed() {
  debugln("Button long press registered.");
  ButtonLongState = 1;
}

/*
bool debounce() {
  bool reading = digitalRead(BUTTON_PIN);

  if (reading != ButtonStateLast) {

    lastDebounce = millis();
  }

  if ((millis() - lastDebounce) > debounceTime) {
    buttonState = reading;
    if (ButtonState == 1) {
      ButtonStateLast = reading;
      return 1;
    }
    buttonStateLast = reading;
  }
  return 0;
}
*/
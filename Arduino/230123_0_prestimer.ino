/* Matthias Kneidinger 24.01.2023
This arduino program attempts to ressamlble a simple countdown/timer 
based on a finite state machine. 
MCU ATtiny85, Adafriut Trinket 5V

This section holds setup
*/

//#include <TM1637Display.h>
#include <TM1637TinyDisplay.h>
#include <RotaryEncoder.h>
#include <OneButton.h>

#define SOFTSERIAL 0

#if SOFTSERIAL == 1
#include <ATtinySerialOut.hpp>  //SoftwareSerial for ATtiny
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debug(x)
#define debugln(x)
#endif

// 7seg Display connection pins (Digital Pins)
#define DISP_CLK_PIN 0
#define DISP_DIO_PIN 3
//TM1637Display display(DISP_CLK_PIN, DISP_DIO_PIN);
TM1637TinyDisplay display(DISP_CLK_PIN, DISP_DIO_PIN);

//
#define ROTARY_A_PIN 1
#define ROTARY_B_PIN 2
#define BUTTON_PIN 4
// A pointer to the dynamic created rotary encoder instance.
// This will be done in setup()
RotaryEncoder *encoder = nullptr;
// Create a button element of the OneButton library
OneButton button(BUTTON_PIN, true);

// This interrupt routine will be called on any change of one of the input pins
void checkInputISR() {
  if (digitalRead(BUTTON_PIN)) {
    button.tick();  //this is part if the oneButton library
  } else {
    encoder->tick();  // just call tick() to check the state, this is part of the RotaryEncoder library
  }
}

// Buttons
bool ButtonClickState = 0;
//bool ButtonClickedLast = 0;

bool ButtonLongState = 0;
//bool ButtonLongPressedLast = 0;

//uint32_t LastDebounce = 0;        //deprecated
//const uint8_t DebounceTime = 50;  //deprecated

// State Machine
enum class timerState : uint8_t {
  MENU,
  COUNTPAUSED,
  COUNTUP,
  COUNTDOWN
};

enum menuItem : uint8_t {
  RESET,
  UP,
  DOWN,
  BACK,
};

static timerState CurrState = timerState::COUNTPAUSED;
static timerState LastState = timerState::MENU;
static menuItem CurrItem = menuItem::RESET;

// Time Counting
static unsigned long timeStart = 0;    // the time from where counting starts
static unsigned long timeReal = 0;     // the up-to-date time of counting
static unsigned long timeDownNow = 0;  // the countdowntime that is left and saved between pauses
static unsigned long timeDownSet = 0;  // the countdowntime the is originally set by the user

// here are all the used functions listed
// ... to be included

/*
// Display - Text in Segments
const PROGMEM uint8_t D_HYPHEN[] = { 0b00000001, 0b00000001, 0b00000001, 0b00000001 };
const PROGMEM uint8_t D_DOWN[] = { 0b00000001, 0b00000001, 0b00000001, 0b00000001 };   //doun
const PROGMEM uint8_t D_UP[] = { 0b00000001, 0b00000001, 0b00000001, 0b00000001 };     //uP
const PROGMEM uint8_t D_BACK[] = { 0b01111111, 0b01110111, 0b01001110, 0b01001110 };   //bAcc
const PROGMEM uint8_t D_RESET[] = { 0b01111111, 0b01110111, 0b01001110, 0b01001110 };  //rSEt

const PROGMEM char Down[] = "doun";  // Must be globally defined for PROGMEM space
const PROGMEM char Up[] = "uP";
const PROGMEM char Back[] = "Bacc";
const PROGMEM char Reset[] = "rSEt";
*/

const uint8_t Letter[13] = {
  0b01110111,  // means A
  0b01111100,  // means b
  0b01011000,  // means c
  0b01011110,  // means d
  0b01111001,  // means E
  0b01010100,  // means n
  0b01011100,  // means o
  0b01110011,  // means P
  0b01010000,  // means r
  0b01101101,  // means S
  0b01111000,  // means t
  0b00011100,  // means u
  0b00000000,  // means <empty>
};

void setup() {

//Serial.begin(115200);
//while (!Serial)
//  ;
#if SOFTSERIAL == 1
  // this bit is for the software serial - needs cleanup!!
  initTXPin();  //defautl TX_PIN is PB2 on an ATtiny85
  debugln(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_ATTINY_SERIAL_OUT));
  writeString("OSCCAL=");
  writeUnsignedByteHexWithPrefix(OSCCAL);

  debugln(F("Praesentationstimer gestartet."));
#endif
  // set brightness between 0 to 7
  display.clear();
  display.setBrightness(BRIGHT_1);
  //display.flipDisplay(false);  // flip if necessary

  // FOUR3, FOUR0, TWO03
  encoder = new RotaryEncoder(ROTARY_A_PIN, ROTARY_B_PIN, RotaryEncoder::LatchMode::TWO03);

  //pinMode(BUTTON_PIN, INPUT_PULLUP); // this declaration is not needed with the OneButton library
  button.attachClick(buttonClicked);
  // TODO: This is deprecated ?
  button.setPressTicks(600);  // that is the time when LongPressStart is called
  button.attachLongPressStart(buttonLongPressed);

  // register interrupt routine
  attachInterrupt(digitalPinToInterrupt(ROTARY_A_PIN), checkInputISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ROTARY_B_PIN), checkInputISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), checkInputISR, CHANGE);
}

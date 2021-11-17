#include <math.h>
#include <EEPROM.h>

#define SYSTEM_STATE_UNLOCKED 0 // point can move freely through the digits
#define SYSTEM_STATE_LOCKED 1 // point is locked on one digit as to edit it

#define JOYSTICK_READING_POSITIVE 1
#define JOYSTICK_READING_NEGATIVE - 1

const int dataPin = 12; // DS
const int latchPin = 11; // STCP
const int clockPin = 10; // SHCP

const int segD1 = 7;
const int segD2 = 6;
const int segD3 = 5;
const int segD4 = 4;

int displayDigits[] = {
  segD1,
  segD2,
  segD3,
  segD4
};

int digitArray[16] = {
  //A B C D E F G DP 
  B11111100, // 0 -> 252
  B01100000, // 1
  B11011010, // 2
  B11110010, // 3
  B01100110, // 4
  B10110110, // 5
  B10111110, // 6
  B11100000, // 7
  B11111110, // 8
  B11110110 // 9
};

int pointPosition = B0010;
int lastPointChangeTime = 0;
bool pointState = 0;
const int pointBlinkingInterval = 500;

const int displayCount = 4;

int digitMemory[] = {0, 0, 0, 0}; // these correspond to the 4 display digits
const int eeAddressDigitMemory = 0;

const int pinX = A4;
const int pinY = A5;
const int pinSW = 3;

bool isJoystickRead = false;
int joystickLowerBound = -250;
int joystickUpperBound = 250;

const int joystickDefaultLowerBound = 0;
const int joystickDefaultUpperBound = 1023;
const int joystickCenteredLowerBound = -512;
const int joystickCenteredUpperBound = 512;

volatile int systemState = 0;
volatile int lastSystemStateChange = 0;
const int debounceTime = 1000;

const int muxDelay = 5;

void changeSystemState() {

  volatile int currentTime = millis();
  if (currentTime - lastSystemStateChange < debounceTime) { 
    return;
  }
    
  lastSystemStateChange = currentTime;
  if (systemState == SYSTEM_STATE_UNLOCKED) {
    systemState = SYSTEM_STATE_LOCKED;
    
    return;
  }

  if (systemState == SYSTEM_STATE_LOCKED) {
    systemState = SYSTEM_STATE_UNLOCKED;
    EEPROM.put(eeAddressDigitMemory, digitMemory);
    return;
  }
}

void writeReg(int digit) {
  
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, digit);
  digitalWrite(latchPin, HIGH);
}

void showDigit(int displayNumber) {

  // turn off all display digits then turn on the one we want
  for (int i = 0; i < displayCount; i++) {
    digitalWrite(displayDigits[i], HIGH);
  }

  digitalWrite(displayDigits[displayNumber], LOW);
}

int log2(int number) {
  
  int answer = 0;
  while (number >>= 1){
    answer +=1;
  }
  return answer;
}

int getDigitPosition(int selection) {
  // get selected digit by doing logarithm of the point position; avoids an extra index value
  return log2(selection);
  
}
void scrollThroughDigit(int direction) {

  int digitPosition = getDigitPosition(pointPosition);
  int value = digitMemory[digitPosition];

  // direction can be either +1 or -1
  value = value + direction;

  if (value > 9) {
    value = 0;
  }

  if (value < 0) {
    value = 9;
  }

  digitMemory[digitPosition] = value;
}

void determinePointState() {
  // if system is locked, show solid point, otherwise blink it
  if (systemState == SYSTEM_STATE_LOCKED) {
    pointState = true;
    return; 
  }
  
  int currentTime = millis();
  if (currentTime - lastPointChangeTime > pointBlinkingInterval) {
    lastPointChangeTime = currentTime;
    pointState = !pointState;
  }
  
}

void writeToDisplay() {
  
  int pointDisplayPosition = pointPosition;
  int currentDigit = 0;
  int pointDigit = 0;
  
  determinePointState();

  for (int i = 0; i < displayCount; i++) {
    pointDigit = pointDisplayPosition % 2;
    pointDisplayPosition /= 2;

    currentDigit = digitArray[digitMemory[i]] | (pointDigit & pointState);
    writeReg(currentDigit);
    showDigit(i);

    delay(muxDelay);
  }
}

// reads joystick value from selected axis
// returns 0 if no reading found, otherwise +-1 for positive/negative action, respectively
int getJoystickReading(int axis) {
  
  int reading = analogRead(axis);
  
  // center the interval as to make values more intuitive
  reading = map(reading, joystickDefaultLowerBound, joystickDefaultUpperBound, joystickCenteredLowerBound, joystickCenteredUpperBound);
  
  if (reading >= joystickLowerBound && reading <= joystickUpperBound){
    return 0; 
  }

  if (reading < joystickLowerBound) {
    return JOYSTICK_READING_NEGATIVE;
  }

  return JOYSTICK_READING_POSITIVE;
  
}

void changeSelection(int reading) {

    // selected digit is controlled using bit shifting
    switch (reading) {
      case JOYSTICK_READING_POSITIVE:
        if (pointPosition < B1000){
          pointPosition = pointPosition << 1;  
          }
        break;
      
      case JOYSTICK_READING_NEGATIVE:
        if (pointPosition > B1){
          pointPosition = pointPosition >> 1;          
        }
        break;
    }
}

void readJoystick() {

  int axis = (systemState == SYSTEM_STATE_UNLOCKED) ? pinX : pinY;
  int reading = getJoystickReading(axis);

  switch (reading) {
  case 0:
    isJoystickRead = false;
    break;
    
  default:
    if (!isJoystickRead) {
      // on joystick move, depending on system state, perform its corresponding action (scroll through digits or change digit value)
      if (systemState == SYSTEM_STATE_UNLOCKED){
        changeSelection(reading);      
      } else {
        scrollThroughDigit(reading);
      }
      isJoystickRead = true;          
    }
    break;
  }

  
}

void setup() {

  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);

  for (int i = 0; i < displayCount; i++) {
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], LOW);
  }

  pinMode(pinSW, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(pinSW), changeSystemState, FALLING);

  EEPROM.get(eeAddressDigitMemory, digitMemory);
  Serial.begin(9600);
}

void loop() {
  readJoystick();
  writeToDisplay();
}
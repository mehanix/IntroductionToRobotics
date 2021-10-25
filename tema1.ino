// potetiometers give us an analog signal so these need to be Analog-In 
const int redPotPin = A2,
          greenPotPin = A1,
          bluePotPin = A0;

// we need to use PWM-enabled pins to emulate analog-out
const int redPin = 3,
          greenPin = 5,
          bluePin = 6;

const int potLowerBound = 0,
          potUpperBound = 1023,
          ledLowerBound = 0,
          ledUpperBound = 255;
         
int redPotValue = 0,
    greenPotValue = 0,
    bluePotValue = 0,
    
    redPinValue = 0,
    greenPinValue = 0,
    bluePinValue = 0;

void setup() {
  pinMode(redPotPin, INPUT);
  pinMode(greenPotPin, INPUT);
  pinMode(bluePotPin, INPUT);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  
}

void loop() {
  // get potentiometer data
  redPotValue = analogRead(redPotPin);
  greenPotValue = analogRead(greenPotPin);
  bluePotValue = analogRead(bluePotPin);

  // get its equivalent in the led interval
  redPinValue = map(redPotValue, potLowerBound, potUpperBound, ledLowerBound, ledUpperBound);
  greenPinValue = map(greenPotValue, potLowerBound, potUpperBound, ledLowerBound, ledUpperBound);
  bluePinValue = map(bluePotValue, potLowerBound, potUpperBound, ledLowerBound, ledUpperBound);

  // send the data to the leds  
  analogWrite(redPin, redPinValue);
  analogWrite(greenPin, greenPinValue);
  analogWrite(bluePin, bluePinValue);

  delay(1);
  
}

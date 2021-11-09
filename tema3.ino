// segment pins
const int pinA = 2;
const int pinB = 3;
const int pinC = 4;
const int pinD = 5;
const int pinE = 6;
const int pinF = 7;
const int pinG = 8;
const int pinDP = 9;

const int segSize = 8;
const int noOfDigits = 10;

// store the pins in an array for easier access
int segments[segSize] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

const int buzzerPin = 10;
const int antennaPin = A5;

byte digitMatrix[noOfDigits][segSize - 1] = {
// a  b  c  d  e  f  g
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}  // 9
};
float antennaValue = 0;
float averaging = 0;

const int sampleCount = 500;
unsigned int sampleIndex = 0;

int buzzerNote, buzzerBaseValue = 200;

void displayNumber(byte digit) {
  
  for (int i = 0; i < segSize - 1; i++) {
    digitalWrite(segments[i], digitMatrix[digit][i]);
  }
}

void setup() {
  
  // initialize all the pins
  for (int i = 0; i < segSize; i++) {
    pinMode(segments[i], OUTPUT);
  }

  pinMode(buzzerPin, OUTPUT);
  pinMode(antennaPin, INPUT);
  Serial.begin(9600);
}

void buzz(int antennaValue) {
  
  // higher pitched note when detecting something
  buzzerNote = antennaValue * buzzerBaseValue;
  tone(buzzerPin, buzzerNote);
}

void loop() {

  if (sampleIndex < sampleCount) {
    averaging += analogRead(antennaPin);
    sampleIndex += 1;
  }

  if (sampleIndex >= sampleCount) {
    antennaValue = (averaging / sampleCount);
    Serial.println(antennaValue);

    antennaValue = constrain(antennaValue, 0, 25);
    antennaValue = map(antennaValue, 0, 25, 0, 9);

    displayNumber(antennaValue);
    buzz(antennaValue);

    averaging = 0;
    sampleIndex = 0;

    // stabilisation
    delay(100);
  }
}

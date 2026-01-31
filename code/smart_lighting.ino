// -------- PIN DEFINITIONS --------
const int pirPin = 2;
const int buttonPin = 3;
const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;

// -------- VARIABLES --------
bool overrideMode = false;
bool lightState = false;

unsigned long lastMotionTime = 0;
const unsigned long timeout = 15000;

const unsigned long bedtime = 120000;
unsigned long startTime;

// -------- SETUP --------
void setup() {
  pinMode(pirPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP); // ✅ FIX

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  turnLightOff();
  startTime = millis();
}

// -------- LOOP --------
void loop() {

  unsigned long currentTime = millis();
  int motion = digitalRead(pirPin);
  int button = digitalRead(buttonPin);

  // -------- OVERRIDE BUTTON --------
  if (button == LOW) { // ✅ pressed
    overrideMode = !overrideMode;
    lightState = overrideMode;
    delay(300); // debounce
  }

  // -------- MANUAL MODE --------
  if (overrideMode) {
    if (lightState) turnLightOn();
    else turnLightOff();
    return;
  }

  // -------- AUTOMATIC MODE --------
  if (motion == HIGH) {
    lightState = true;
    lastMotionTime = currentTime;
  }

  if (lightState && (currentTime - lastMotionTime > timeout)) {
    lightState = false;
  }

  if (currentTime - startTime > bedtime) {
    lightState = false;
  }

  if (lightState) turnLightOn();
  else turnLightOff();
}

// -------- LIGHT FUNCTIONS --------
void turnLightOn() {
  analogWrite(redPin, 255);
  analogWrite(greenPin, 245);
  analogWrite(bluePin, 205);
}

void turnLightOff() {
  analogWrite(redPin, 0);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 0);
}

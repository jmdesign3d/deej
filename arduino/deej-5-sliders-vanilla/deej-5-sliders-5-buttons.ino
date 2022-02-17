#include <Keyboard.h>

#include <HID.h>

const int NUM_SLIDERS = 5;
const int NUM_BUTTONS = 5;
const int analogInputs[NUM_SLIDERS] = {A0, A1, A2, A3, A6};
const int Inputs[NUM_BUTTONS] = {5, 6, 7, 8, 9};
const int fKeys[NUM_BUTTONS] = {0xF0, 0xF1, 0xF2, 0xF3, 0xF4};

int analogSliderValues[NUM_SLIDERS];
int buttonValues[NUM_BUTTONS];
int legacyButtonValues[NUM_BUTTONS] = {1, 1, 1, 1, 1};

void setup() { 
  for (int i = 0; i < NUM_SLIDERS; i++) {
    pinMode(analogInputs[i], INPUT);
  }

  for (int i = 0; i < NUM_BUTTONS; i++) {
    pinMode(Inputs[i], INPUT_PULLUP);
  }
  pinMode(15, OUTPUT);
  Serial.begin(9600);
  Keyboard.begin();
}

void loop() {
  updateSliderValues();
  checksendButtonValues();
  sendSliderValues(); // Actually send data (all the time)
  //printSliderValues(); // For debug
  delay(100);
}


void updateSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
     analogSliderValues[i] = analogRead(analogInputs[i]);
  }
}

void sendSliderValues() {
  String builtString = String("");

  for (int i = 0; i < NUM_SLIDERS; i++) {
    builtString += String((int)analogSliderValues[i]);

    if (i < NUM_SLIDERS - 1) {
      builtString += String("|");
    }
  }
  
  Serial.println(builtString);
}


void checksendButtonValues() {
  for (int i = 0; i < NUM_BUTTONS; i++) {
     buttonValues[i] = digitalRead(Inputs[i]);
     if (buttonValues[i] == 0)  {
      if (legacyButtonValues[i] != buttonValues[i])  {
        Keyboard.write(fKeys[i]);
      }
     }
  legacyButtonValues[i] = buttonValues[i];
  }
}


void printSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
    String printedString = String("Slider #") + String(i + 1) + String(": ") + String(analogSliderValues[i]) + String(" mV");
    Serial.write(printedString.c_str());

    if (i < NUM_SLIDERS - 1) {
      Serial.write(" | ");
    } else {
      Serial.write("\n");
    }
  }
}

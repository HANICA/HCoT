/*
  Sketch om een met een Teensy (Arduino compatible ARM) (www.pjrc.com)
  de gegevens van een Sharp GP2Y1010AU0F stofsensor te lezen

  Based on code by Cyrille Médard de Chardon (serialC), Christophe Trefois (Trefex) from
  https://github.com/Trefex/arduino-airquality/
*/

#define sIDLE 0
#define sIRON 1
#define sDELTA 2
#define sIROFF 3

const int measurePin = A6;
const int ledPower = 12;
const int heartBeat = 13;

const int samplingTime = 280;
const int deltaTime = 40;
const int sleepTime = 9680;

const int sensorDelayTime = 1000;

float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;

int dustSenseState = 0; // 0: IDLE, 1: IR ON, 2: deltatime, 3: IR LED off


long dustSenseTimer = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ledPower, OUTPUT);
  pinMode(heartBeat, OUTPUT);
}

void loop() {
  takeSamples();
}





/*
  Sketch om een met een Teensy (Arduino compatible ARM) (www.pjrc.com)
  de gegevens van een Sharp GP2Y1010AU0F stofsensor te lezen

  Based on code by Cyrille Médard de Chardon (serialC), Christophe Trefois (Trefex) from
  https://github.com/Trefex/arduino-airquality/
*/

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
#define sIDLE 0
#define sIRON 1
#define sDELTA 2
#define sIROFF 3

long dustSenseTimer = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ledPower, OUTPUT);
  pinMode(heartBeat, OUTPUT);
}

void loop() {
  takeSamples();
  /*
  digitalWrite(ledPower, LOW); // power on the LED
  delayMicroseconds(samplingTime);

  voMeasured = analogRead(measurePin); // read the dust value

  delayMicroseconds(deltaTime);
  digitalWrite(ledPower, HIGH); // turn the LED off
  delayMicroseconds(sleepTime);

  // 0 - 3.3V mapped to 0 - 1023 integer values
  // recover voltage
  calcVoltage = voMeasured * (3.3 / 1024);

  // linear eqaution taken from http://www.howmuchsnow.com/arduino/airquality/
  // Chris Nafis (c) 2012
  dustDensity = 0.17 * calcVoltage - 0.1;

  Serial.print("Raw Signal Value (0-1023): ");
  Serial.print(voMeasured);

  Serial.print(" - Voltage: ");
  Serial.print(calcVoltage);

  Serial.print(" - Dust Density: ");
  Serial.println(dustDensity);*/
}

void takeSamples() {
  boolean transistion = false;
  if (dustSenseState == sIDLE) {
    if (millis() > dustSenseTimer + sensorDelayTime) {
      dustSenseState++;
      dustSenseTimer = micros();
      digitalWrite(ledPower, LOW);
    }
  } else if (dustSenseState == sIRON) {
    if (micros() > dustSenseTimer + samplingTime) {
      dustSenseState++;
      dustSenseTimer = micros();
      voMeasured = analogRead(measurePin);
    }
  } else if (dustSenseState == sDELTA) {
    if (micros() > dustSenseTimer + deltaTime) {
      dustSenseState++;
      dustSenseTimer = micros();
      digitalWrite(ledPower, HIGH);
    }
  } else if (dustSenseState == sIROFF) {
    if (micros() > dustSenseTimer + sleepTime) {
      dustSenseState = 0;
      dustSenseTimer = millis();
      dustDensity = 0.17 * calcVoltage - 0.1;
      Serial.print("Raw Signal Value (0-1023): ");
      Serial.print(voMeasured);

      Serial.print(" - Voltage: ");
      Serial.print(calcVoltage);

      Serial.print(" - Dust Density: ");
      Serial.println(dustDensity);
    }
  }
}


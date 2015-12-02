/*
  Sketch om een met een Teensy (Arduino compatible ARM) (www.pjrc.com)
  de gegevens van een Sharp GP2Y1010AU0F stofsensor te lezen
  Hogeschool van Arnhem en Nijmegen
  Johan Korten, okt. 2015
  v1.0

  Based on code by Cyrille Médard de Chardon (serialC), Christophe Trefois (Trefex) from
  https://github.com/Trefex/arduino-airquality/
  http://arduino-er.blogspot.nl/2015/05/arduino-esp8266-simpe-web-server.html
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
const int sleepTime = 9680; // 10.000µS - samplingTime (280µS) - sleepTime (40µS)

const int sensorDelayTime = 100; // na elke elke seconde lezen (in millisecondes)

float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;

int dustSenseState = 0; // 0: IDLE, 1: IR ON, 2: deltatime, 3: IR LED off


long dustSenseTimer = 0;

unsigned long heartBeatTimer = 0;
int heartBeatTime = 5000;
int heartBeatTimeOff = 100;
volatile float meetwaarde = 0.0;

void setup() {
  Serial.begin(9600);
  pinMode(ledPower, OUTPUT);
  pinMode(heartBeat, OUTPUT);
  initWifi();
}

void loop() {
  float sensordata = takeSample();
  
  
  if (sensordata > 0.0) {
    meetwaarde = (meetwaarde + sensordata)/2;
  }
  checkWifi(meetwaarde);
  //wifiAction();
  //flashHeartBeat();
}

void flashHeartBeat() {
  if (millis() - heartBeatTimer > heartBeatTime) {
    digitalWrite(heartBeat, HIGH);
    if (millis() - heartBeatTimer > heartBeatTime + heartBeatTimeOff) {
      heartBeatTimer = millis();
      digitalWrite(heartBeat, LOW);
    }
  }
}





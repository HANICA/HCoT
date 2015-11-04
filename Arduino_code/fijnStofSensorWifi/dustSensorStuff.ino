void takeSamples() {
  boolean transistion = false;
  if (dustSenseState == sIDLE) {
    if (millis() - dustSenseTimer > sensorDelayTime) { // slaaptijd tussen leescycles
      dustSenseState = sIRON;
      dustSenseTimer = micros();
      digitalWrite(ledPower, LOW); // sensor gaat dan AAN (via transistor, AAN = LOW)
    }
  } else if (dustSenseState == sIRON) {
    if (micros() - dustSenseTimer > samplingTime) { 
      dustSenseState = sDELTA;
      dustSenseTimer = micros();
      voMeasured = analogRead(measurePin);
    }
  } else if (dustSenseState == sDELTA) {
    if (micros() - dustSenseTimer > deltaTime) {
      dustSenseState = sIROFF;
      dustSenseTimer = micros();
      digitalWrite(ledPower, HIGH); // sensor gaat dan UIT (via transistor, UIT = HIGH)
    }
  } else if (dustSenseState == sIROFF) {
    if (micros() - dustSenseTimer > sleepTime) { // minimale slaap tijd
      dustSenseState = sIDLE;
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




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




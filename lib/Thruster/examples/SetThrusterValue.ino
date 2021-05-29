#include "Arduino.h"
#include "Thruster.h"

#define MIN_VALUE 0
#define MAX_VALUE 255

Thruster thruster(MIN_VALUE, MAX_VALUE);

void setup() {
  Serial.begin(9600);

  cli();              // stop interrupts
  thruster.attach(9); // attach motor
  sei();              // allow interrupts
}

void loop() {
  if (Serial.available())
    thruster.setValue(Serial.parseInt());
}
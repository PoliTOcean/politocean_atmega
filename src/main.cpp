#include "Arduino.h"
#include "MPU6050.h"
#include "SPI.h"
#include "Wire.h"

volatile bool received = false, reset = false;
volatile byte c;
volatile int currentAxis = 0;
volatile byte buffer[6], seq[6] = {1, 2, 3, 4, 5, 6};

void printBuffer(volatile byte *b, size_t size);

MPU6050 mpu;
cartesian_t acc, gyro;
cartesian_t accOffset = {.x = 5115, .y = 419, .z = 533},
            gyroOffset = {.x = -21, .y = 592, .z = 584};

void setup() {
  Serial.begin(9600);
  mpu.begin();

  Serial.println("Calibrating...");
  mpu.calibrate();
  Serial.println(" Ok.");

  pinMode(MISO, OUTPUT);
  SPCR |= _BV(SPE);
  SPI.attachInterrupt();
}

void loop() {
  mpu.readAccelerometer(&acc);
  Serial.print("Accelerometer: "), Serial.print(acc.x), Serial.print(" "),
      Serial.print(acc.y), Serial.print(" "), Serial.println(acc.z);
  mpu.readGyroscope(&gyro);
  Serial.print("Gyroscope: "), Serial.print(gyro.x), Serial.print(" "),
      Serial.print(gyro.y), Serial.print(" "), Serial.println(gyro.z);

  delay(1000);
}

ISR(SPI_STC_vect) {
  c = SPDR;

  if (c == 0xff) {
    reset = true;
    currentAxis = 0;
  } else {
    buffer[currentAxis] = c;

    if (++currentAxis >= 6) {
      currentAxis = 0;
      received = true;
    }
  }
}

void printBuffer(volatile byte *b, size_t size) {
  for (unsigned int i = 0; i < size; i++) {
    Serial.print(b[i], DEC), Serial.print('\t');
  }
  Serial.println("");
}
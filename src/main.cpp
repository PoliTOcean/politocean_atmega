#include "Arduino.h"
<<<<<<< HEAD
#include "Matlib.h"
#include "Engine.h"

=======
#include "MPU6050.h"
#include "SPI.h"
#include "Wire.h"
>>>>>>> origin/master

volatile bool received = false, reset = false;
volatile byte c;
volatile int currentAxis = 0;
volatile byte buffer[6], seq[6] = {1, 2, 3, 4, 5, 6};

<<<<<<< HEAD
void print2(const Matrix<2> &a);
int xin1 = A0;
int yin1 = A1;
int x, y;
double inputToPower[6] = {};
double outputToPower[6] = {};
double outputDistribute[7] = {};
int outputPWM[7] = {};
Matrix<2> input = {1, 1};
Reference r = Reference{};
Engine e = Engine{};


void setup() { 
  Serial.begin(9600); 
  
}

void loop() {

  auto tStart = micros();
  //integrator.setInput(input);
  //integrator.step();
  //Matrix<2> newState = integrator.getState();
  //print2(newState);
  x = analogRead(xin1);
  y = analogRead(yin1);
  //Serial.print("Analog read: ");
  //Serial.print(x);
  //Serial.print(" ");
  //Serial.println(y);
  
  for(int i = 0; i < 6; i++){
    inputToPower[i] = 127.0;
  }



  
  inputToPower[0] = (double)x * 255.0/ 1023.0;
  inputToPower[1] = (double)y * 255.0 / 1023.0;

  inputToPower[1] = 255.0;
  
  //Serial.print("Input: ");
  for(int i = 0; i < 6; i++){
      if( abs(inputToPower[i] - 127.0) < 10.0){
         inputToPower[i] = 127.0;
      }
      //Serial.print(inputToPower[i]);
      //Serial.print(" ");
  }
  //Serial.println();
  
  
  r.toPower(inputToPower, outputToPower);
  
  /*
  Serial.print("TOPOWER: ");
  for(int i = 0; i < 6; i++){

      Serial.print(outputToPower[i]);
      Serial.print(" ");
  }
  Serial.println();
  */
  
  e.distribute(outputToPower, 0, 0, 0, 0, 0, 0, outputDistribute);
  Serial.print("PWM: ");
  for(int i = 0; i < 7; i++){
      outputPWM[i] = e.computePWM(outputDistribute[i]);
      Serial.print(outputPWM[i]);
      Serial.print(" ");
  }
  Serial.println();
  
  
  auto tEnd = micros();
  Serial.print("TEnd-Tstart: "), Serial.println(tEnd-tStart);
  delay(200);
=======
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
>>>>>>> origin/master
}

void printBuffer(volatile byte *b, size_t size) {
  for (unsigned int i = 0; i < size; i++) {
    Serial.print(b[i], DEC), Serial.print('\t');
  }
  Serial.println("");
}
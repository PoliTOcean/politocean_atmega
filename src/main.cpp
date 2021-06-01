#include "Arduino.h"
#include "Matlib.h"
#include "Wire.h"
#include "Engine.h"
#include "lqr.h"
#include "MPU6050.h"

using namespace matlib;

volatile bool received = false, reset = false;
volatile byte c;
volatile int currentAxis = 0;
volatile byte buffer[6], seq[6] = {1, 2, 3, 4, 5, 6};

void print2(const Matrix<2> &a);
int xin1 = A0;
int yin1 = A1;
int x, y;
double inputToPower[6];
double outputToPower[6];
double outputDistribute[7];
int outputPWM[7];
Matrix<2> input = {1, 1};
Reference r;
Engine e;
LQR l;
MPU6050 mpu6050;
cartesian_t acc, gyro;
Integrator<6> integratedMeasurements(
    Matrix<6>::zeros(),
    0.01);

void setup() { 
  Wire.begin();
  Serial.begin(9600); 
  mpu6050.begin();
  mpu6050.calibrate();
}

void loop() {

  auto tStart = micros();
  mpu6050.readGyroscope(&gyro);
  mpu6050.readAccelerometer(&acc);

  //Matrix<6,1> read = {acc.x, acc.y, acc.z, gyro.x, gyro.y, gyro.z};
  Matrix<6,1> read = {0, 0, 0, 0, 0, 0};

  integratedMeasurements.setInput(read);
  integratedMeasurements.step();
  Matrix<6,1> tmpMeasures = integratedMeasurements.getState();

  x = analogRead(xin1);
  y = analogRead(yin1);
  
  for(int i = 0; i < 6; i++){
    inputToPower[i] = 127.0;
  }



  
  inputToPower[0] = (double)x * 255.0/ 1023.0;
  inputToPower[1] = (double)y * 255.0 / 1023.0;

  inputToPower[0] = 255.0;
  
  for(int i = 0; i < 6; i++){
      if( abs(inputToPower[i] - 127.0) < 10.0){
         inputToPower[i] = 127.0;
      }
  }
  
  Matrix<6,1> commands = {inputToPower[0], inputToPower[1], inputToPower[2], 
                          inputToPower[3], inputToPower[4], inputToPower[5]};

  r.toPower(inputToPower, outputToPower);
  
  e.distribute(outputToPower, e.addEnvironment(l.compute(l.measurements(tmpMeasures, commands)), 
                                                       tmpMeasures(3,0), 
                                                       tmpMeasures(4, 0), 
                                                       tmpMeasures(5, 0)), 
                                                       outputDistribute);

  int outputPWM[7];
  e.computePWM(outputDistribute, outputPWM);
  auto tEnd = micros();
  Serial.print("TEnd-Tstart: "), Serial.println(tEnd-tStart);
  Serial.print("PWM: ");
  
  for (int i= 0; i < 7; i++){
    Serial.print(outputPWM[i]);
    Serial.print(" ");
  }
  Serial.println();
  delay(200);
}

void printBuffer(volatile byte *b, size_t size) {
  for (unsigned int i = 0; i < size; i++) {
    Serial.print(b[i], DEC), Serial.print('\t');
  }
  Serial.println("");
}
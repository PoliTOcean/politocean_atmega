#include "Arduino.h"
#include "Matlib.h"
#include "Engine.h"


using namespace matlib;

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
}

void print2(const Matrix<2> &a) {
  Serial.print(a(0)), Serial.print('\t'), Serial.println(a(1));
}
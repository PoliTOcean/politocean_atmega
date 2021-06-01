#include "engine.h"
#include <math.h>
#include <string.h>
#include "Arduino.h"

using namespace matlib;

void Reference::toPower(const double perc[6], double vect_Forces[6]) {
  double G[6];
  double q[6];
  double p[6];
  int i;
  
  static const Matrix<2, 6> Gain = {
      0.6391799880331942,    0.6391799880331942, 0.1780734772993898, 
      0.057206610392085536,  0.10546713649209566, 0.20076246365902112,
      0.50443995571260791,   0.50443995571260791, 0.46899584428269525,
      0.038698589382881392,  0.0068670248575934522, 0.15844145651138775};

  // we can extert a much higher force in this direction, since we don't have to
  // fight buoyancy computing m and q of the line that relates the % of the
  // joytick and the Ft here the percentage has been computed as 0-255. If you
  // want to get it from -100-100, substitute (255-128)->100, 127->100 Gain
  // positivi Gain negativi

  //Serial.print("G: ");
  for(int i = 0; i < 6; i++){
      if( abs(perc[i] - 127.0) < 10.0){
         p[i] = 127.0;
      }
      else{
        p[i] = perc[i];
      }
  }
  
  for (i = 0; i < 6; i++) {
    if (p[i] >= 128.0) {
      G[i] = Gain(0, i);

      q[i] = G[i] * 127.0;
    } else {
      
      G[i] = Gain(1, i);
      q[i] = G[i] * 127.0;
    }

    //Serial.print(G[i]);
    //Serial.print(" ");
    vect_Forces[i] = G[i] * p[i] - q[i];
  }

  /*Serial.println();
  Serial.print("q: ");

  for(int j = 0; j<6; j++){
    Serial.print(q[j]);
    Serial.print(" ");
  }

  Serial.println();
  */
}

/* Function Definitions */
/*
 * Arguments    : const double vect_Forces[6]
 *                double Ftx
 *                double Fty
 *                double Ftz
 *                double Mtx
 *                double Mty
 *                double Mtz
 *                double y[7]
 * Return Type  : void
 */
void Engine::distribute(const double vect_Forces[6], Matrix<6,1> torques,
                        double y[7]) {
  double F1_Ftx = 0.0;
  double F1_Fty = 0.0;
  double F1_Mtz = 0.0;
  double F2_Mtz = 0.0;
  double F7_Ftx = 0.0;
  double y_tmp = 0.0;
  torques(0, 0) += vect_Forces[0];
  torques(1, 0) += vect_Forces[1];
  torques(2, 0) += vect_Forces[2];
  torques(3, 0) += vect_Forces[3];
  torques(4, 0) += vect_Forces[4];
  torques(5, 0) += vect_Forces[5];

  /* Due to Ftx */
  F7_Ftx = -0.1698 * torques(0, 0);
  F1_Ftx = 0.3536 * torques(0, 0);

  /* Due to Fty */
  F1_Fty = -torques(1, 0) / 2.8284271247461903;

  /* Due to Ftz */
  /* Due to Mtx */
  /* VERIFICARE */
  /* Due to Mty */
  /* Due to Mz */
  F1_Mtz = -1.08325 * torques(5, 0);
  F2_Mtz = 1.12563 * torques(5, 0);

  /* ???? */
  y_tmp = F1_Ftx + F1_Fty;
  y[0] = y_tmp + F1_Mtz;
  F1_Ftx -= F1_Fty;
  y[1] = F1_Ftx + F2_Mtz;
  y[2] = F1_Ftx - F2_Mtz;
  y[3] = y_tmp - F1_Mtz;
  y[4] = ((0.000201 * torques(1, 0) + 0.380273 * torques(2, 0)) + 0.0049 * torques(3, 0)) + -3.0386 * torques(4, 0);
  y[5] = (((-F7_Ftx + -0.110565 * torques(1, 0)) + 0.313408 * torques(2, 0)) + -2.57849 * torques(3, 0)) +
         1.52721 * torques(4, 0);
  y[6] = (((F7_Ftx + 0.11364 * torques(1, 0)) + 0.306319 * torques(2, 0)) + 2.5739 * torques(3, 0)) +
         1.5114 * torques(4, 0);
}

Matrix<6,1> Engine::addEnvironment(Matrix<6,1> torques,  double roll, double pitch,
                            double yaw) {
    
// accelerazione di gravità
double g= 9.80666;        // [(kg*m)/(s^2)]

// massa ROV
double m= 9.94104;          // [kg]


// volume ROV
double V= 6544278.18/pow(1000.0,3);     // [m^3]

// rho=997;  % [kg/m^3]
double rho=997;

double buoyancy=rho*g*V;

// Distanza centro di massa- centro di galleggiamento VERIFICARE!!
double xb= 69.55/1000;     // [m]
double yb=1.01/1000;       // [m]
double zb= 52.31/1000;     // [m]



Matrix<6,8> B_f= {-1, 0, 0, 0, 0, 0, -m*g*cos(yaw)*sin(pitch), m*g*cos(yaw)*sin(pitch),
    0, -1, 0, 0, 0, 0,  m*g*cos(yaw)*sin(roll), -m*g*cos(yaw)*sin(roll),
    0, 0, -1, 0, 0, 0, m*g*cos(roll)*cos(pitch), -m*g*cos(roll)*cos(pitch),
    0, 0, 0, -1, 0, 0, 0, -buoyancy*zb*cos(pitch)*sin(roll),
    0, 0, 0, 0, -1, 0, 0, - buoyancy*zb*sin(pitch)-buoyancy*xb*cos(roll)*cos(pitch),
    0, 0, 0, 0, 0, -1,0, buoyancy*xb*cos(pitch)*sin(roll)};

Matrix<8, 1> T = {torques(0, 0),
                  torques(1, 0),
                  torques(2, 0),
                  torques(3, 0),
                  torques(4, 0),
                  torques(5, 0), 1, 1};

Matrix<6, 1> F_thrust = B_f*T;

/*
Serial.println("B_f");
for(int i = 0; i < 6; i++){
  for(int j = 0; j< 8; j++){
    Serial.print(B_f(i, j));
    Serial.print(" ");
  }
  Serial.println();
}

Serial.print("Formula ");
Serial.println(V);


Serial.print("F_thrust: ");
    for(int i = 0; i < 6; i++){
      Serial.print(torques(i, 0));
      Serial.print(" ");
    }
    Serial.println();
Serial.print("Roll "), Serial.print(roll),
Serial.print("Pitch "), Serial.print(pitch), 
Serial.print("Yaw "), Serial.print(yaw);
*/

return F_thrust;
}

void Engine::computePWM(double u[7], int outputPWM[7]) {
  static const double p_x1[7] = {2.946851474459042e-06,
                                 2.836712230035537e-04,
                                 0.010616408671209,
                                 0.199901189414592,
                                 2.176683385925845,
                                 26.369976813191993,
                                 1.465861832468260e+03};
  static const double p_x2[7] = {-6.645745311609482e-07,
                                  8.239563077992088e-05,
                                  -0.003935635978640,
                                  0.094003405635814,
                                  -1.311652566479037,
                                  20.751467116095448,
                                  1.534679140574547e+03};



  
  for(int i = 0; i < 7; i++){
      if (u[i] <= 0) {
          outputPWM[i] = (int)(p_x1[0] * pow(u[i], 6) + p_x1[1] * pow(u[i], 5) + p_x1[2] * pow(u[i], 4) +
          p_x1[3] * pow(u[i], 3) + p_x1[4] * pow(u[i], 2) + p_x1[5] * u[i] + p_x1[6]);
      } else {
          outputPWM[i] = (int)(p_x2[0] * pow(u[i], 6) + p_x2[1] * pow(u[i], 5) + p_x2[2] * pow(u[i], 4) +
          p_x2[3] * pow(u[i], 3) + p_x2[4] * pow(u[i], 2) + p_x2[5] * u[i] + p_x2[6]);
      }

      if(outputPWM[i] > SATURATION_PWM_MAX) outputPWM[i] = SATURATION_PWM_MAX;
      else if (outputPWM[i] < SATURATION_PWM_MIN) outputPWM[i] = SATURATION_PWM_MIN;
  }
}
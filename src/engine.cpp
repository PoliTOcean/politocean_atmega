#include "engine.h"
#include <math.h>
#include <string.h>
#include "Arduino.h"

using namespace matlib;

void Reference::toPower(const double perc[6], double vect_Forces[6]) {
  double G[6];
  double q[6];
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
  for (i = 0; i < 6; i++) {
    if (perc[i] >= 128.0) {
      G[i] = Gain(0, i);

      q[i] = G[i] * 127.0;
    } else {
      
      G[i] = Gain(1, i);
      q[i] = G[i] * 127.0;
    }

    //Serial.print(G[i]);
    //Serial.print(" ");
    vect_Forces[i] = G[i] * perc[i] - q[i];
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
void Engine::distribute(const double vect_Forces[6], double Ftx, double Fty,
                        double Ftz, double Mtx, double Mty, double Mtz,
                        double y[7]) {
  double F1_Ftx;
  double F1_Fty;
  double F1_Mtz;
  double F2_Mtz;
  double F7_Ftx;
  double y_tmp;
  Ftx += vect_Forces[0];
  Fty += vect_Forces[1];
  Ftz += vect_Forces[2];
  Mtx += vect_Forces[3];
  Mty += vect_Forces[4];
  Mtz += vect_Forces[5];

  /* Due to Ftx */
  F7_Ftx = -0.1698 * Ftx;
  F1_Ftx = 0.3536 * Ftx;

  /* Due to Fty */
  F1_Fty = -Fty / 2.8284271247461903;

  /* Due to Ftz */
  /* Due to Mtx */
  /* VERIFICARE */
  /* Due to Mty */
  /* Due to Mz */
  F1_Mtz = -1.08325 * Mtz;
  F2_Mtz = 1.12563 * Mtz;

  /* ???? */
  y_tmp = F1_Ftx + F1_Fty;
  y[0] = y_tmp + F1_Mtz;
  F1_Ftx -= F1_Fty;
  y[1] = F1_Ftx + F2_Mtz;
  y[2] = F1_Ftx - F2_Mtz;
  y[3] = y_tmp - F1_Mtz;
  y[4] = ((0.000201 * Fty + 0.380273 * Ftz) + 0.0049 * Mtx) + -3.0386 * Mty;
  y[5] = (((-F7_Ftx + -0.110565 * Fty) + 0.313408 * Ftz) + -2.57849 * Mtx) +
         1.52721 * Mty;
  y[6] = (((F7_Ftx + 0.11364 * Fty) + 0.306319 * Ftz) + 2.5739 * Mtx) +
         1.5114 * Mty;
}

void Engine::addEnvironment(double Fx, double Fy, double Fz, double Mx,
                            double My, double Mz, double roll, double pitch,
                            double yaw, double F_thrust[6]) {
  double dv[48];
  double b_Fx[8];
  double B_f_tmp;
  double b_B_f_tmp;
  double c_B_f_tmp;
  double d_B_f_tmp;
  double e_B_f_tmp;

  //  cos(45) e sin(45)= angoli asse dei motori
  //  accelerazione di gravità
  //  [(kg*m)/(s^2)]
  //  massa ROV
  //  [kg]
  //  volume ROV
  //  [m^3]
  //  rho=997;  % [kg/m^3]
  //  Distanza centro di massa- centro di galleggiamento VERIFICARE!!
  //  [m]
  //  [m]
  //  [m]
  B_f_tmp = cos(yaw);
  b_B_f_tmp = sin(pitch);
  c_B_f_tmp = sin(roll);
  d_B_f_tmp = cos(roll);
  e_B_f_tmp = cos(pitch);
  dv[0] = -1.0;
  dv[6] = 0.0;
  dv[12] = 0.0;
  dv[18] = 0.0;
  dv[24] = 0.0;
  dv[30] = 0.0;
  dv[36] = -106.2061278 * B_f_tmp * b_B_f_tmp;
  dv[42] = 106.2061278 * B_f_tmp * b_B_f_tmp;
  dv[1] = 0.0;
  dv[7] = -1.0;
  dv[13] = 0.0;
  dv[19] = 0.0;
  dv[25] = 0.0;
  dv[31] = 0.0;
  dv[37] = 106.2061278 * cos(yaw) * c_B_f_tmp;
  dv[43] = -106.2061278 * cos(yaw) * c_B_f_tmp;
  dv[2] = 0.0;
  dv[8] = 0.0;
  dv[14] = -1.0;
  dv[20] = 0.0;
  dv[26] = 0.0;
  dv[32] = 0.0;
  dv[38] = 106.2061278 * d_B_f_tmp * e_B_f_tmp;
  dv[44] = -106.2061278 * d_B_f_tmp * e_B_f_tmp;
  dv[3] = 0.0;
  dv[9] = 0.0;
  dv[15] = 0.0;
  dv[21] = -1.0;
  dv[27] = 0.0;
  dv[33] = 0.0;
  dv[39] = 0.0;
  dv[45] = -4.9658456175244785 * e_B_f_tmp * c_B_f_tmp;
  dv[4] = 0.0;
  dv[10] = 0.0;
  dv[16] = 0.0;
  dv[22] = 0.0;
  dv[28] = -1.0;
  dv[34] = 0.0;
  dv[40] = 0.0;
  dv[46] = -4.9658456175244785 * b_B_f_tmp -
           6.6024577078728246 * d_B_f_tmp * e_B_f_tmp;
  dv[5] = 0.0;
  dv[11] = 0.0;
  dv[17] = 0.0;
  dv[23] = 0.0;
  dv[29] = 0.0;
  dv[35] = -1.0;
  dv[41] = 0.0;
  dv[47] = 6.6024577078728246 * e_B_f_tmp * c_B_f_tmp;
  b_Fx[0] = Fx;
  b_Fx[1] = Fy;
  b_Fx[2] = Fz;
  b_Fx[3] = Mx;
  b_Fx[4] = My;
  b_Fx[5] = Mz;
  b_Fx[6] = 1.0;
  b_Fx[7] = 1.0;
  for (int i = 0; i < 6; i++) {
    B_f_tmp = 0.0;
    for (int i1 = 0; i1 < 8; i1++) {
      B_f_tmp += dv[i + 6 * i1] * b_Fx[i1];
    }

    F_thrust[i] = B_f_tmp;
  }
}

int Engine::computePWM(double u) {
  static const double p_x1[7] = {5.85047836804337e-06, 0.000534932248223446,
                                 0.0188426982631957,   0.332359754817047,
                                 3.32059076815697,     33.6082379110387,
                                 1463.84980390593};
  static const double p_x2[7] = {-2.07964488262876e-06, 0.000238770996705692,
                                 -0.0104230998524322,   0.223304777305886,
                                 -2.61239351684421,     28.5351019056098,
                                 1535.28905301730};

  int p_x = 0;
  if (u <= 0) {
    p_x = p_x1[0] * pow(u, 6) + p_x1[1] * pow(u, 5) + p_x1[2] * pow(u, 4) +
          p_x1[3] * pow(u, 3) + p_x1[4] * pow(u, 2) + p_x1[5] * u + p_x1[6];
  } else {
    p_x = p_x2[0] * pow(u, 6) + p_x2[1] * pow(u, 5) + p_x2[2] * pow(u, 4) +
          p_x2[3] * pow(u, 3) + p_x2[4] * pow(u, 2) + p_x2[5] * u + p_x2[6];
  }

  if(p_x > SATURATION_PWM_MAX) p_x = SATURATION_PWM_MAX;
  else if (p_x < SATURATION_PWM_MIN) p_x = SATURATION_PWM_MIN;


  return (int)p_x;
}
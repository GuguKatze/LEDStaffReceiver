#include "Globals.h"
float pitchFiltered = 0;
float xAccFiltered = 0;

signed int state = -1;
long stateSince = 0;
unsigned int stateSpinning = 0;
unsigned int stateUp = 0;
unsigned int stateDown = 0;
unsigned int stateOther = 0;

void imuLogic() {
  float xAcc, yAcc, zAcc, xGyro, yGyro, zGyro;
  IMU.readAcceleration(xAcc, yAcc, zAcc);
  IMU.readGyroscope(xGyro, yGyro, zGyro);
  filter.updateIMU(xGyro, yGyro, -zGyro, xAcc, yAcc, -zAcc);
  float pitch = filter.getPitch();
  //pitchFiltered = pitchFiltered * 0.99 + pitch * 0.01;
  pitchFiltered = pitchFiltered * 0.9 + pitch * 0.1;
  //xAccFiltered  = xAccFiltered  * 0.99 +  xAcc * 0.01;
  xAccFiltered  = xAccFiltered  * 0.9 +  xAcc * 0.1;

  // 3 = spinning yes/no
  // 4 = vertical yes/no
  // 5 = up is up
 
}

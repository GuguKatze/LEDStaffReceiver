#include "Globals.h"

float pitch;

signed int state = -1;
long stateSince = 0;
unsigned int stateSpinning = 0;
unsigned int stateUp = 0;
unsigned int stateDown = 0;
unsigned int stateOther = 0;

void topImpulse(){    digitalWrite( 3,  LOW); digitalWrite( 4, HIGH); digitalWrite( 5, HIGH); digitalWrite( 6, HIGH); Serial.println("4 -> 1"); }
void rightImpulse(){  digitalWrite( 3, HIGH); digitalWrite( 4,  LOW); digitalWrite( 5, HIGH); digitalWrite( 6, HIGH); Serial.println("1 -> 2"); }
void bottomImpulse(){ digitalWrite( 3, HIGH); digitalWrite( 4, HIGH); digitalWrite( 5,  LOW); digitalWrite( 6, HIGH); Serial.println("2 -> 3"); }
void leftImpulse(){   digitalWrite( 3, HIGH); digitalWrite( 4, HIGH); digitalWrite( 5, HIGH); digitalWrite( 6,  LOW); Serial.println("3 -> 4"); }



void imuLogic() {
  float xAcc, yAcc, zAcc, xGyro, yGyro, zGyro;
  IMU.readAcceleration(xAcc, yAcc, zAcc);
  IMU.readGyroscope(xGyro, yGyro, zGyro);
  filter.updateIMU(xGyro, yGyro, -zGyro, xAcc, yAcc, -zAcc);
  pitch = filter.getPitch();
  //return pitch;
/*
if(stateSpinning > 0){ stateSpinning--; }
if(      stateUp > 0){ stateUp      --; }
if(    stateDown > 0){ stateDown    --; }
if(   stateOther > 0){ stateOther   --; }

if(xAcc > 1.2 || xAcc < -1.2){
  if(stateSpinning < 128){ stateSpinning++; }
}else if(pitch > 70){
  if(stateUp < 128)      { stateUp++; }
}else if(pitch < -70){
  if(stateDown < 128)    { stateDown++; }
}else{
  if(stateOther < 128)   { stateOther++; }
}

if(stateSpinning > stateUp && stateSpinning > stateDown && stateSpinning > stateOther){
  if(state != 0 && millis() - stateSince > 2000){
    state = 0;
    stateSince = millis();
    Serial.println(state);  
  }
}else if(stateUp > stateSpinning && stateUp > stateDown && stateUp > stateOther){
  if(state != 1 && millis() - stateSince > 2000){
    state = 1;
    stateSince = millis();
    Serial.println(state);  
  }
}else if(stateDown > stateUp && stateDown > stateSpinning && stateDown > stateOther){
  if(state != 2 && millis() - stateSince > 2000){
    state = 2;
    stateSince = millis();
    Serial.println(state);  
  }
}else if(stateOther > stateUp && stateOther > stateSpinning && stateOther > stateDown){
  if(state != 3 && millis() - stateSince > 2000){
    state = 3;
    stateSince = millis();
    Serial.println(state);  
  }
  
}
*/

}

#include "Globals.h"
int peakIndicator = 0;
float pitchFlattest = 100;
unsigned short state = 4;
unsigned long lastImuPrint = 0;
float pitch;
float pitchMax = 0; float pitchMaxLast = 0; unsigned long topTime    = 0; unsigned long topLastTime    = 0;    // TOP
float pitchMin = 0; float pitchMinLast = 0; unsigned long bottomTime = 0; unsigned long bottomLastTime = 0; // BOTTOM
unsigned long rightToBottom = 0; unsigned long leftToTop = 0;
unsigned long lastTopImpulse = 0; unsigned long lastRightImpulse = 0; unsigned long lastBottomImpulse = 0; unsigned long lastLeftImpulse = 0;

void topImpulse(){    digitalWrite( 3,  LOW); digitalWrite( 4, HIGH); digitalWrite( 5, HIGH); digitalWrite( 6, HIGH); lastTopImpulse    = millis(); state = 1; Serial.println("4 -> 1"); }
void rightImpulse(){  digitalWrite( 3, HIGH); digitalWrite( 4,  LOW); digitalWrite( 5, HIGH); digitalWrite( 6, HIGH); lastRightImpulse  = millis(); state = 2; Serial.println("1 -> 2"); peakIndicator = 0; }
void bottomImpulse(){ digitalWrite( 3, HIGH); digitalWrite( 4, HIGH); digitalWrite( 5,  LOW); digitalWrite( 6, HIGH); lastBottomImpulse = millis(); state = 3; Serial.println("2 -> 3"); }
void leftImpulse(){   digitalWrite( 3, HIGH); digitalWrite( 4, HIGH); digitalWrite( 5, HIGH); digitalWrite( 6,  LOW); lastLeftImpulse   = millis(); state = 4; Serial.println("3 -> 4"); peakIndicator = 0; }

void imuLogic() {
  float xAcc, yAcc, zAcc, xGyro, yGyro, zGyro; IMU.readAccel(xAcc, yAcc, zAcc); IMU.readGyro(xGyro, yGyro, zGyro); filter.updateIMU(xGyro, yGyro, -zGyro, xAcc, yAcc, -zAcc);
  pitch = filter.getPitch();
  float pitchFiltered = pitch;
  // TOP
  if (state == 4 && pitchFiltered > 20) {
    if (pitchFiltered > pitchMax) {
      pitchMax = pitchFiltered; topTime  = millis();
    }else if(pitchFiltered < pitchMax - 5){
      state = 1;
      if(lastLeftImpulse != 0){ leftToTop = topTime - lastLeftImpulse; Serial.println("topTime: " + String(topTime) + " lastLeftImpulse: " + String(lastLeftImpulse) + " leftToTop: " + String(leftToTop)); }
    }
    if(pitchFlattest != 100){ pitchFlattest = 100; }
  }
  // BOTTOM
  if (state == 2 && pitchFiltered < -20) {
    if (pitchFiltered < pitchMin) {
      pitchMin     = pitchFiltered; bottomTime = millis();
    }else if(pitchFiltered > pitchMin + 5){
      state = 3;
      if(lastRightImpulse != 0){ rightToBottom = bottomTime - lastRightImpulse; Serial.println("bottomTime: " + String(bottomTime) + " lastRightImpulse: " + String(lastRightImpulse) + " rightToBottom: " + String(rightToBottom)); }
    }
    if(pitchFlattest != 100){ pitchFlattest = 100; }
  }
  // HORIZONTAL
  if((state == 1 || state == 3) && fabs(pitchFiltered) < 5){
    if(pitch <= 0 && state == 1){
      Serial.println("+++ rightImpulse");
      rightImpulse();
    }else if(pitch >= 0 && state == 3){
      Serial.println("+++ leftImpulse");
      leftImpulse();
    }
    if(pitchMax != 0){pitchMax = 0; }
    if(pitchMin != 0){pitchMin = 0; }
  }
  if(state == 2 && rightToBottom       != 0 && lastRightImpulse  != 0 && lastRightImpulse  + rightToBottom < millis()){
    Serial.println("~~~ bottomImpulse");
    bottomImpulse();
  }else if(state == 4 && leftToTop     != 0 && lastLeftImpulse   != 0 && lastLeftImpulse   + leftToTop     < millis()){
    Serial.println("~~~ topImpulse");
    topImpulse();
  }  
  if (millis() - lastImuPrint > 500) {
    peakIndicator = (state - 2) * 40;
    //Serial.println(String(pitchFiltered) + "," + String(pitchMinLast) + "," + String(pitchMaxLast) + "," + String(peakIndicator) + "," + String(state));
    lastImuPrint = millis();
  }
}

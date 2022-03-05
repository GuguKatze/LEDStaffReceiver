#include "Globals.h"
float pitchFiltered = 0;
float xAccFiltered = 0;

unsigned long lastStateCheckTime = 0;
bool boolVertical = false;
bool boolWasVertical = false;
unsigned long sinceTime = 0;

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

if(millis() - lastStateCheckTime > 250){
  lastStateCheckTime = millis();

  if(pitchFiltered >  30){                       //digitalWrite( 8,  LOW); digitalWrite( 9, HIGH); digitalWrite( 10, HIGH); //Serial.println("Vertical UP.");
    if(boolWasVertical){
      if(millis() - sinceTime > 2000){
        //digitalWrite( 8,  LOW);
        digitalWrite( 9, HIGH); digitalWrite( 10, HIGH); //Serial.println("Vertical UP.");
      }
    }else{
      boolWasVertical = true;
      sinceTime = millis();
    }
  }else if(pitchFiltered < -30){                 //digitalWrite( 8,  LOW); digitalWrite( 9, HIGH); digitalWrite( 10,  LOW); //Serial.println("Vertical DOWN.");
    if(boolWasVertical){
      if(millis() - sinceTime > 2000){
        //digitalWrite( 8,  LOW);
        digitalWrite( 9, HIGH); digitalWrite( 10, LOW); //Serial.println("Vertical DOWN.");
      }
    }else{
      boolWasVertical = true;
      sinceTime = millis();
    }
  }else{                                         //digitalWrite( 8,  LOW); digitalWrite( 9,  LOW); digitalWrite( 10,  LOW); //Serial.println("Other.");
    if(!boolWasVertical){
      if(millis() - sinceTime > 2000){
        //digitalWrite( 8,  HIGH);
        digitalWrite( 9, LOW); digitalWrite( 10, LOW); //Serial.println("Horizontal.");
      }
    }else{
      boolWasVertical = false;
      sinceTime = millis();
    }
  }
  /*
  float xaf = xAccFiltered * 10;
  Serial.print(pitchFiltered);
  Serial.print(",");
  Serial.print(xaf);
  Serial.println(",-90,90");
  */
  }
}

/*
   writeRegister(LSM9DS1_ADDRESS, LSM9DS1_CTRL_REG1_G, 0x98); // 119 Hz, 2000 dps, 16 Hz BW
  // https://www.st.com/resource/en/datasheet/lsm9ds1.pdf 45
  // default = 0x78 = 0111 1000
  //                  1001 1000 = 0x98
  
  writeRegister(LSM9DS1_ADDRESS, LSM9DS1_CTRL_REG6_XL, 0x90); // 119 Hz, 4G
  // https://www.st.com/resource/en/datasheet/lsm9ds1.pdf 52
  // default = 0x70 = 0111 0000
  //                  1001 0000 = 0x90
  
 */


  /* 
    // Knock detection:
    if(millis() - knockDebounce > 1000){
      knockCount = 0;
    }
    if((xAcc < -2 || xAcc > 2) && millis() - knockDebounce > 100){
      knockDebounce = millis();
      if(knockCount == 0 || millis() - knockDebounce <= 500){
        knockCount = knockCount + 1;
        if(useSerial){ Serial.println("KNOCK!!! " + String(knockCount)); }
        if(knockCount == 3){
          knockCount = 0;
          ledsEnabled = !ledsEnabled;
        }
      }
    }
 */   


 /*
  * ORANGE SCL NANO A5 ---> Teensy4.1 A5
  * YELLOW SDA NANO A4 ---> Teensy4.1 A4
  */

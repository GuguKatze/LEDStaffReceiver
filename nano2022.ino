#include "Globals.h"
#include "ImuLogic.h"
Madgwick filter;
//const float sensorRate = 238.00;
const float sensorRate = 104.00;
unsigned long microsPerReading = 0;
unsigned long microsPrevious = 0;

bool useSerial = true;
bool viewInSerialPlotter = true;


/////////////
// packets //
/////////////
union effectPacket_ effectPacket;
union pitchPacket_  pitchPacket;
union vuPacket_     vuPacket;

union I2Cdata_ I2Cdata;
uint8_t peaks = 0;
unsigned long vuLastSignal = 0;

#include <ArduinoBLE.h>
BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); // BLE LED Service
BLECharacteristic vuCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite, sizeof(vu.bytes));

uint8_t ledRed   = 4;
uint8_t ledGreen = 2;
uint8_t ledBlue  = 3;
unsigned long ledBlueLastTime = 0;

void setup() {

  // statusLED
  pinMode(  ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode( ledBlue, OUTPUT);
  analogWrite(  ledRed,  0);
  analogWrite(ledGreen,  0);
  analogWrite( ledBlue,  0);
  
  //delay(3000); if(useSerial){ Serial.begin(115200); }; delay(1000);
  delay(3000); if(useSerial){ Serial.begin(9600); }; delay(1000);
  if(useSerial && !Serial){ delay(1000); if(!Serial){useSerial = false; }; }
  if(useSerial){ Serial.println("Starting up ..."); };

  // BLE
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1);
  }
  BLE.setLocalName("LED");
  BLE.setAdvertisedService(ledService);
  ledService.addCharacteristic(vuCharacteristic);
  BLE.addService(ledService);
  BLE.advertise();
  String address = BLE.address();
  Serial.print("BLE Peripheral @ ");
  Serial.println(address);

  // IMU
  while(!IMU.begin()) { delay(1000); };
  filter.begin(sensorRate);
  microsPerReading = 1000000 / sensorRate;

  /*
  Wire.begin(0x40);
  Wire.setClock(1000000);
  Wire.onRequest(requestEvent);
  */
  Wire.begin(); // master
  Wire.setClock(1000000);
  
  pinMode(3, OUTPUT); // top  
  digitalWrite(3, HIGH ); 
  pinMode(4, OUTPUT); // right
  digitalWrite(4, HIGH);
  pinMode(5, OUTPUT); // bottom
  digitalWrite(5, HIGH );
  pinMode(6, OUTPUT); // left
  digitalWrite(6, HIGH );
  
  pinMode(7, OUTPUT); // vu
  digitalWrite(7, HIGH );
  Serial.println("Setup finished ...");
}

void loop() {
  // listen for BLE peripherals to connect:
  BLEDevice central = BLE.central();
  
  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());

    // while the central is still connected to peripheral:
    while (central.connected()) {
/*
  BLEDevice central = BLE.central();
  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());
    if(central.connected()) {
    */
      if(random(0,10000) == 0){
        Wire.beginTransmission(0x40);
        union effectPacket_ effectPacket;
        effectPacket.packetType = 1;
        effectPacket.effect = 5;
        Serial.println("[I2C] Sending packet. Type: effectPacket");
        Wire.write(effectPacket.bytes, sizeof(effectPacket.bytes));
        Wire.endTransmission(); 
      }
      
      /////////
      // BLE //
      /////////
      if(millis() - ledBlueLastTime > 100){
        analogWrite( ledBlue, 0);
      }
      if(vuCharacteristic.written()) {
        analogWrite( ledBlue, 24);
        ledBlueLastTime = millis();
        //Serial.println("[BLE] Received packet. Type: pitchPacket");

        byte packetType;
        vuCharacteristic.readValue(packetType);
        if(packetType == 1){
          vuCharacteristic.readValue(effectPacket.bytes, sizeof(effectPacket.bytes));
          //memcpy(&effectPacket.bytes[1], &vu.bytes[0], sizeof(vu.bytes));
          Serial.println("[BLE] Received packet. Type: effect");
        }else if(packetType == 2){
          vuCharacteristic.readValue(pitchPacket.bytes, sizeof(pitchPacket.bytes));
          Serial.println("[BLE] Received packet. Type: pitch");
          Serial.println(pitchPacket.pitch);
        }else if(packetType == 3){
           vuCharacteristic.readValue(vuPacket.bytes, sizeof(vuPacket.bytes));
           Serial.println("[BLE] Received packet. Type: vu");
        }
        /**
        uint8_t packetType[1];
        vuCharacteristic.readValue(packetType, 1);
        Serial.println(packetType[0]);
        
        uint8_t packetType2[1];
        vuCharacteristic.readValue(packetType2, 1);        
        Serial.println(packetType2[0]);
        **/
        
        //uint8_t packetBuffer[32];
        //vuCharacteristic.readValue(packetBuffer, 32);
        //uint8_t packetType = packetBuffer[0];

        
        /*
        unsigned int byteCount = vuCharacteristic.readValue(vu.bytes, sizeof(vu.bytes));
        vuLastSignal = millis();
        digitalWrite( 7, LOW);

        Wire.beginTransmission(0x40);
        union vuPacket_ vuPacket;
        memcpy(&vuPacket.bytes[1], &vu.bytes[0], sizeof(vu.bytes));
        Serial.println("[SENDING]vuPacket ...");
        Wire.write(vuPacket.bytes, sizeof(vuPacket.bytes));
        Wire.endTransmission();
        
        Serial.print("<");
        for(int i=0;i<7;i++){
          Serial.print(max((vuPacket.left[i]),0));
          Serial.print(":");
          Serial.print(max((vuPacket.right[i]),0));
          if(i<6){ Serial.print(", "); }else{ Serial.println(); }
        }
       
      }
      if(millis() - vuLastSignal > 200){
        digitalWrite( 7, HIGH);
      }
      */
      }
    }
  }

  //if(IMU.accelAvailable() && IMU.gyroAvailable()){
  unsigned long microsNow = micros();
  if(microsNow - microsPrevious >= microsPerReading) {
    imuLogic();
  }
}

void requestEvent()
{
  /* 
  memcpy(&I2Cdata.bytes[0], &vu.bytes[0], sizeof(vu.bytes));
  I2Cdata.pitch = (int8_t) pitch;
  //Serial.println("pitch: " + String(I2Cdata.pitch));
  //Wire.write(vu.bytes, sizeof(vu.bytes));
  Wire.write(I2Cdata.bytes, sizeof(I2Cdata.bytes));
 */
  if(random(0, 2) == 1){
    Wire.write(pitchPacket.bytes, sizeof(pitchPacket.bytes));
  }else{
    Wire.write(effectPacket.bytes, sizeof(effectPacket.bytes));
  }
}

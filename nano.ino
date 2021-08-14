#include "Globals.h"
#include "ImuLogic.h"
Madgwick filter;
const float sensorRate = 238.00;
unsigned long microsPerReading = 0;
unsigned long microsPrevious = 0;

bool useSerial = true;
bool viewInSerialPlotter = true;

unsigned long lastTest = 0;

union vu_ {
  struct __attribute__((packed)){
    int8_t pitch;
    //uint16_t left[7];
    uint8_t left[7];
    //uint16_t right[7];
    uint8_t right[7];
  };
  //uint8_t bytes[28];
  //uint8_t bytes[14];
  uint8_t bytes[15];
};
union vu_ vu;

#include <ArduinoBLE.h>
BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); // BLE LED Service
BLECharacteristic vuCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite, sizeof(vu.bytes));

void setup() {
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
 
  IMU.setAccelFS(1);           
  IMU.setAccelODR(4);
  IMU.setAccelOffset(-0.010616, -0.023320, 0.002512);
  IMU.setAccelSlope (0.992297, 0.991011, 0.996108);

  IMU.setGyroFS(2);
  IMU.setGyroODR(4);
  IMU.setGyroOffset (1.197083, 5.412812, -0.005676);
  IMU.setGyroSlope (1.173538, 1.173056, 1.144210);

  IMU.gyroUnit=DEGREEPERSECOND; // DEGREEPERSECOND  RADIANSPERSECOND  REVSPERMINUTE  REVSPERSECOND  
  microsPerReading = 1000000 / sensorRate;
  Wire.begin(0x40);
  Wire.onRequest(requestEvent);
  pinMode(3, OUTPUT); // top  
  digitalWrite(3, HIGH ); 
  pinMode(4, OUTPUT); // right
  digitalWrite(4, HIGH);
  pinMode(5, OUTPUT); // bottom
  digitalWrite(5, HIGH );
  pinMode(6, OUTPUT); // left
  digitalWrite(6, HIGH );
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
      if (vuCharacteristic.written()) {
        unsigned int byteCount = vuCharacteristic.readValue(vu.bytes, sizeof(vu.bytes));
        //Serial.println(String(byteCount));
        //Serial.print("[<---]");
        Serial.print("<");
        /*
        for(int i=0;i<7;i++){
          Serial.print(max((vu.left[i]),0));
          Serial.print(":");
          Serial.print(max((vu.right[i]),0));
          if(i<6){ Serial.print(", "); }else{ Serial.println(); }
        }
        */
      }
    }
  }

  //if(IMU.accelAvailable() && IMU.gyroAvailable()){
  if (!viewInSerialPlotter){
    Serial.print("Gyroscope Full Scale = ±");
    Serial.print(IMU.getGyroFS());
    Serial.println ("°/s");
    Serial.print("Gyroscope sample rate = ");
    Serial.print(IMU.getGyroODR());        //alias  IMU.gyroscopeSampleRate());
    Serial.println(" Hz");
    Serial.print("Accelerometer Full Scale = ±");
    Serial.print(IMU.getAccelFS());
    Serial.println ("g");
    Serial.print("Accelerometer sample rate = ");
    Serial.print(IMU.getAccelODR());        // alias  AccelerationSampleRate());
    Serial.println(" Hz \n");
  }  
  unsigned long microsNow = micros();
  if(microsNow - microsPrevious >= microsPerReading) {
    imuLogic();
  }
}

void requestEvent()
{
  //Serial.println("[--->]");
  vu.pitch = (int) pitch;
  Serial.println("> " + String(vu.pitch));
  Wire.write(vu.bytes, sizeof(vu.bytes));
}

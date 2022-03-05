#pragma once
#include <Wire.h>
#define SCL A5
#define SDA A4
#include "ImuLogic.h"
extern Madgwick filter;
extern bool useSerial;
extern float pitchFiltered;
extern float xAccFiltered;
////////////
// effect //
////////////
union effectPacket_ {
  struct __attribute__((packed)){
    uint8_t packetType = 1;
    uint8_t effect;
  };
  uint8_t bytes[2];
};
extern union effectPacket_ effectPacket;
///////////
// pitch //
///////////
union pitchPacket_ {
  struct __attribute__((packed)){
    uint8_t packetType = 2;
    int8_t pitch;
  };
  uint8_t bytes[2];
};
extern union pitchPacket_ pitchPacket;
////////
// vu //
////////
union vuPacket_ {
  struct __attribute__((packed)){
    uint8_t packetType = 3;
    uint8_t left[7];
    uint8_t right[7];
  };
  uint8_t bytes[15];
};
extern union vuPacket_ vuPacket;
/////////////////
// pitchRemote //
/////////////////
union pitchRemotePacket_ {
  struct __attribute__((packed)){
    uint8_t packetType = 4;
    int8_t pitch;
  };
  uint8_t bytes[2];
};
extern union pitchRemotePacket_ pitchRemotePacket;

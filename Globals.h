#pragma once
#include <Wire.h>
#define SCL A5
#define SDA A4
#include "ImuLogic.h"
extern Madgwick filter;
extern bool useSerial;
extern bool viewInSerialPlotter;
extern float pitch;

union I2Cdata_ {
  struct __attribute__((packed)){
    uint8_t left[7];
    uint8_t right[7];
    int8_t pitch;
  };
  uint8_t bytes[15];
};
extern union I2Cdata_ I2Cdata;

union vu_ {
  struct __attribute__((packed)){
    uint8_t left[7];
    uint8_t right[7];
  };
  uint8_t bytes[14];
};
extern union vu_ vu;

#pragma once
#include <Wire.h>
#define SCL A5
#define SDA A4
#include "ImuLogic.h"
extern Madgwick filter;
extern bool useSerial;
extern bool viewInSerialPlotter;
extern float pitch;

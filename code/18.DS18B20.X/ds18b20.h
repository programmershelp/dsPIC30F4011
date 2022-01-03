#ifndef __DS18B20_H
#define __DS18B20_H
#include "p30f4011.h"
#define   DQ PORTFbits.RF6
extern unsigned int ReadTemperature(void);
#endif

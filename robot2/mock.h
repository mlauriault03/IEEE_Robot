#ifdef DEBUG

#ifndef MOCK_FOR_TESTING
#define MOCK_FOR_TESTING

#include "robot2.h"

extern int step_writes_fl;
extern int step_writes_fr;
extern int step_writes_bl;
extern int step_writes_br;


void delay(unsigned long ms);
void delayMicroseconds(unsigned int us);
unsigned long millis();
unsigned long micros();

#endif
#endif

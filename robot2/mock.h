#ifdef DEBUG

#ifndef MOCK_FOR_TESTING
#define MOCK_FOR_TESTING

#include "robot2.h"

extern int nsteps_fl;
extern int nsteps_fr;
extern int nsteps_bl;
extern int nsteps_br;

void delay(unsigned long ms);
void delayMicroseconds(unsigned int us);
unsigned long millis();
unsigned long micros();

#endif
#endif

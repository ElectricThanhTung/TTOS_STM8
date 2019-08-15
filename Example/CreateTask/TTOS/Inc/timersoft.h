
// Written By : Thanh Tung
// E-Mail     : electricthanhtung@gmail.com
// Facebook   : https://www.facebook.com/electricthanhtung

#ifndef __TIMER_H__
#define __TIMER_H__

#include "ttos.h"

typedef struct {
  unsigned char ID;
  void (*Func)();
  unsigned int Time;
} Timer;

Timer TTOS_CreateTimer(void (*func)(), unsigned int time_ms);
unsigned char TTOS_ClearTimer(Timer func);

#endif

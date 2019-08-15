
// Written By : Thanh Tung
// E-Mail     : electricthanhtung@gmail.com
// Facebook   : https://www.facebook.com/electricthanhtung

#include "timersoft.h"

static void TimerTask(unsigned int *arg){
  void (*func)() = (void (*)())arg[0];
  unsigned int time = arg[1];
  while(1){
    TTOS_Sleep(time);
    func();
  }
}

Timer TTOS_CreateTimer(void (*func)(), unsigned int time_ms){
  static unsigned int arg[2];
  Task task;
  Timer Res;
  arg[0] = (unsigned int)func;
  arg[1] = time_ms;
  task = TTOS_CreateTask(TimerTask, arg);
  Res.ID = task.ID;
  Res.Func = task.Func;
  Res.Time = time_ms;
  return Res;
}

unsigned char TTOS_ClearTimer(Timer timer){
  Task task;
  task.ID = timer.ID;
  task.Func = timer.Func;
  return TTOS_ClearTask(task);
}

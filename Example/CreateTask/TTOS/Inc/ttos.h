
// Written By : Thanh Tung
// E-Mail     : electricthanhtung@gmail.com
// Facebook   : https://www.facebook.com/electricthanhtung

#ifndef __TTOS_H__
#define __TTOS_H__

#define RAMSIZE                 2048
#define STACKSIZE               128
#define TCPU                    50

typedef struct {
  unsigned char ID;
  void (*Func)();
} Task;

void TTOS_Init();
void TTOS_StartKernel();
void TTOS_StopKernel();
void TTOS_SwitchTask();
Task This();
Task TTOS_CreateTask(void (*func)(), void const *arg);
unsigned char TTOS_ClearTask(Task task);
unsigned char TTOS_Sleep(unsigned int time_ms);
unsigned char TTOS_PauseTask(Task task);
unsigned char TTOS_ResumeTask(Task task);
unsigned char TTOS_IsRunning(Task task);
unsigned char TTOS_IsExist(Task task);

#endif

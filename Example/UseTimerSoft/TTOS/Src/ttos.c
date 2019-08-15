
// Written By : Thanh Tung
// E-Mail     : electricthanhtung@gmail.com
// Facebook   : https://www.facebook.com/electricthanhtung

#include <intrinsics.h>
#include "main.h"
#include "ttos.h"

#define MAXTASK                                (RAMSIZE / STACKSIZE) - 2

unsigned char Running = 1;
unsigned char ListRun[MAXTASK];
volatile unsigned char CurrentIndex = 0;
volatile unsigned char CurrentID = 0;
volatile unsigned int SPRun[MAXTASK + 1];

void TTOS_Init(){
  for(unsigned char i = 0; i < MAXTASK; i++)
    SPRun[i] = 0;
  
  TIM4_ARR = TCPU;
  TIM4_PSCR = 0x04;
  TIM4_IER = 0x01;
  TIM4_CR1 = 0x81;
  
  TIM2_ARRH = (1004 >> 8);
  TIM2_ARRL = (unsigned char)1004;
  TIM2_PSCR = 0x04;
  TIM2_IER = 0x01;
  TIM2_CR1 = 0x81;
  
  ITC_SPR1 = 0x00;
  ITC_SPR2 = 0x00;
  ITC_SPR3 = 0x00;
  ITC_SPR4 = 0x00;
  ITC_SPR5 = 0x00;
  ITC_SPR6 = 0x40;
  ITC_SPR7 = 0x00;
  
  __enable_interrupt();
}

#pragma vector = TIM4_OVR_UIF_vector
__interrupt void TIM4_ISR(){
  TIM4_SR = 0x00;
   
  asm("PUSH 0x00");
  asm("PUSH 0x01");
  asm("PUSH 0x02");
  asm("PUSH 0x03");
  asm("PUSH 0x04");
  asm("PUSH 0x05");
  asm("PUSH 0x06");
  asm("PUSH 0x07");
  asm("PUSH 0x08");
  asm("PUSH 0x09");
  asm("PUSH 0x0A");
  asm("PUSH 0x0B");
  
  asm("CLRW X");
  asm("LD A, CurrentID");
  asm("LD XL, A");
  asm("SLAW X");
//  asm("CLRW Y");
  asm("LDW Y, SP");
  asm("LDW (SPRun, X), Y");
  
  if(++CurrentIndex >= Running)
    CurrentIndex = 0;
  CurrentID = ListRun[CurrentIndex];
  
  asm("CLRW X");
  asm("LD A, CurrentID");
  asm("LD XL, A");
  asm("SLAW X");
  asm("LDW X, (SPRun, X)");
  asm("LDW SP, X");
  
  asm("POP 0x0B");
  asm("POP 0x0A");
  asm("POP 0x09");
  asm("POP 0x08");
  asm("POP 0x07");
  asm("POP 0x06");
  asm("POP 0x05");
  asm("POP 0x04");
  asm("POP 0x03");
  asm("POP 0x02");
  asm("POP 0x01");
  asm("POP 0x00");
}

#pragma vector = TIM2_OVR_UIF_vector
__interrupt void TIM2_ISR() {
  for(unsigned char i = 1; i < MAXTASK; i++){
    if(SPRun[i] != 0){
      unsigned int Point = RAMSIZE - i * STACKSIZE - 1;
      if((*(unsigned int *)Point)){
        (*(unsigned int *)Point)--;
        if((*(unsigned int *)Point) == 0){
          ListRun[Running] = i;
          Running++;
        }
      }
    }
  }
  TIM2_SR1 = 0;
}

void TTOS_StartKernel(){
  TIM4_IER = 0x01;
}

void TTOS_StopKernel(){
  TIM4_CNTR = 0x01;
  TIM4_SR = 0x00;
  TIM4_IER = 0x00;
}

void TTOS_SwitchTask(){
  TIM4_EGR = 0x01;
  //asm("NOP");
}

unsigned char TTOS_ClearTask(Task task){
  if(task.ID == 0)
    return 0;
  TTOS_StopKernel();
  for(unsigned char i = 1; i < Running; i++){
    if(task.ID == ListRun[i]){
      Running--;
      ListRun[i] = ListRun[Running];
      break;
    }
  }
  TTOS_StartKernel();
  SPRun[task.ID] = 0;
  if(task.ID == CurrentID){
    CurrentID = MAXTASK;
    TTOS_SwitchTask();
  }
  return 1;
}

Task TTOS_This(){
  Task Res;
  Res.ID = CurrentID;
  unsigned int Point = RAMSIZE - CurrentIndex * STACKSIZE - 3;
  Res.Func = (void (*)())*(unsigned int *)Point;
  return Res;
}

static void EndTask(){
  TTOS_ClearTask(TTOS_This());
}

Task TTOS_CreateTask(void (*func)(), void const *arg){
  Task Res;
  unsigned int SP_Temp;
  TTOS_StopKernel();
  for(unsigned char i = 1; i < MAXTASK; i++){
    if(SPRun[i] == 0){
      SP_Temp = RAMSIZE - i * STACKSIZE - 1;
      *(unsigned int *)SP_Temp = 0;                            // Time Sleep
      SP_Temp -= 2;
      *(unsigned int *)SP_Temp = (unsigned int)func;
      SP_Temp--;
      *(unsigned char *)SP_Temp = 0;
      SP_Temp -= 2;
      *(unsigned int *)SP_Temp = (unsigned int)EndTask;        // Endpoint function
      SP_Temp -= 2;
      *(unsigned int *)SP_Temp = (unsigned int)func;
      SP_Temp--;
      *(unsigned char *)SP_Temp = (unsigned long)func >> 16;
      SP_Temp -= 4;
      *(unsigned int *)SP_Temp = (unsigned int)arg;            // Arguments 
      SP_Temp -= 2;
      *(unsigned char *)SP_Temp = 0x20;
      SP_Temp -= 13;
      SPRun[i] = SP_Temp;
      ListRun[Running] = i;
      Running++;
      TTOS_StartKernel();
      Res.ID = i;
      Res.Func = func;
      return Res;
    }
  }
  TTOS_StartKernel();
  Res.ID = 0;
  Res.Func = 0;
  return Res;
}

unsigned char TTOS_Sleep(unsigned int time_ms){
  if((Running > 1) && (time_ms > 0)){
    unsigned int Point = RAMSIZE - CurrentID * STACKSIZE - 1;
    *(unsigned int *)Point = time_ms;
    TTOS_StopKernel();
    Running--;
    ListRun[CurrentIndex] = ListRun[Running];
    TTOS_StartKernel();
    TTOS_SwitchTask();
    return 1;
  }
  return 0;
}

unsigned char TTOS_ResumeTask(Task task){
  if(!task.ID)
    return 1;
  if(!SPRun[task.ID])
    return 0;
  for(unsigned char i = 1; i < Running; i++){
    if(task.ID == ListRun[i])
      return 1;
  }
  TTOS_StopKernel();
  ListRun[Running] = task.ID;
  Running++;
  TTOS_StartKernel();
  return 1;
}

unsigned char TTOS_PauseTask(Task task){
  if(!task.ID)
    return 0;
  if(!SPRun[task.ID])
    return 0;
  unsigned int Point = RAMSIZE - task.ID * STACKSIZE - 1;
  *(unsigned int *)Point = 0;
  for(unsigned char i = 1; i < Running; i++){
    if(task.ID == ListRun[i]){
      TTOS_StopKernel();
      Running--;
      ListRun[i] = ListRun[Running];
      TTOS_StartKernel();
      TTOS_SwitchTask();
      return 1;
    }
  }
  return 1;
}

unsigned char TTOS_IsRunning(Task task){
  if(!task.ID)
    return 1;
  for(unsigned char i = 1; i < Running; i++){
    if(task.ID == ListRun[i])
      return 1;
  }
  return 0;
}

unsigned char TTOS_IsExist(Task task){
  if(!task.ID)
    return 1;
  if(task.ID >= MAXTASK)
    return 0;
  if(SPRun[task.ID])
    return 1;
  return 0;
}


// Written By : Thanh Tung
// E-Mail     : electricthanhtung@gmail.com
// Facebook   : https://www.facebook.com/electricthanhtung

#include "main.h"

void func1(){
  while(1){
    // ...
  }
}

void func2(){
  while(1){
    // ...
  }
}

void func3(){
  TTOS_Sleep(1000);
  TTOS_CreateTask(func2, 0);
}

void main(){
  CLK_CKDIVR = 0x00;
  TTOS_Init();
  TTOS_CreateTask(func1, 0);
  TTOS_CreateTask(func3, 0);
  while(1){
    TTOS_SwitchTask();
  }
}


// Written By : Thanh Tung
// E-Mail     : electricthanhtung@gmail.com
// Facebook   : https://www.facebook.com/electricthanhtung

#include "main.h"

Task ThanhTung;

void func1(){
  while(1){
    // ...
  }
}

void func2(){
  TTOS_Sleep(1000); // Sleep 1s
  TTOS_ClearTask(ThanhTung);
}

void main(){
  CLK_CKDIVR = 0x00;
  TTOS_Init();
  ThanhTung = TTOS_CreateTask(func1, 0);
  TTOS_CreateTask(func2, 0);
  while(1){
    TTOS_SwitchTask();
  }
}

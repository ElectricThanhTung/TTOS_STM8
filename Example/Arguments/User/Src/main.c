
// Written By : Thanh Tung
// E-Mail     : electricthanhtung@gmail.com
// Facebook   : https://www.facebook.com/electricthanhtung

#include "main.h"

void func1(unsigned char *arg){
  volatile unsigned char a = arg[0]; // = 123;
  volatile unsigned char b = arg[1]; // = 45;
  volatile unsigned char c = arg[2]; // = 78;
  while(1){
    // ...
  }
}

void main(){
  static unsigned char ThanhTung[] = {123, 45, 78};
  CLK_CKDIVR = 0x00;
  TTOS_Init();
  TTOS_CreateTask(func1, ThanhTung);
  while(1){
    TTOS_SwitchTask();
  }
}

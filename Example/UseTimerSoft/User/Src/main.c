
// Written By : Thanh Tung
// E-Mail     : electricthanhtung@gmail.com
// Facebook   : https://www.facebook.com/electricthanhtung

#include "main.h"

#define LED             PD_ODR_ODR0

void GPIO_Init(){
  PD_DDR_DDR0 = 1;
  PD_CR1_C10 = 1;
  PD_CR2_C20 = 1;
}

void LEDBlink(){
  TTOS_StopKernel();
  LED ^= 1;
  TTOS_StartKernel();
}

void main(){
  CLK_CKDIVR = 0x00;
  GPIO_Init();
  TTOS_Init();
  TTOS_CreateTimer(LEDBlink, 500);   // LED Blink 500ms (1Hz)
  while(1){
    TTOS_SwitchTask();
  }
}

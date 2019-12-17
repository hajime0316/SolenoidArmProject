#include "cppmain.hpp"

#include "main.h"
#include "tim.h"
#include "gpio.h"

#define CONTROL_LOOP_TIME 0.01 // sec

void setup(void)
{
}

void loop(void)
{
}

//**************************
//    タイマ割り込み関数
//**************************
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  // TIM6 10msecタイマ
  if (htim->Instance == TIM6) {
  }

  // TIM7 100msecタイマ
  else if (htim->Instance == TIM7) {
  }
}

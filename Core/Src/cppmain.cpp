#include "cppmain.hpp"

#include "main.h"
#include "tim.h"
#include "gpio.h"

#include "stm32_printf/stm32_printf.h"

#define CONTROL_LOOP_TIME 0.01 // sec

void setup(void)
{
}

void loop(void)
{
  if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_RESET) {
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(SOLENOID_GPIO_Port, SOLENOID_Pin, GPIO_PIN_SET);
  }
  else {
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SOLENOID_GPIO_Port, SOLENOID_Pin, GPIO_PIN_RESET);
  }
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

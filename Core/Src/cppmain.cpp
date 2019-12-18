#include "cppmain.hpp"

#include "main.h"
#include "tim.h"
#include "gpio.h"
#include "usart.h"

#include "stm32_printf/stm32_printf.h"

#define CONTROL_LOOP_TIME 0.01 // sec
#define SET_DUTY_RATE_SERVO_0(duty_rate) (htim1.Instance->CCR1 = SERVO_PWM_PERIOD * duty_rate)
#define SET_DUTY_RATE_SERVO_1(duty_rate) (htim1.Instance->CCR2 = SERVO_PWM_PERIOD * duty_rate)

void setup(void)
{
  stm32_printf_init(&huart1);

  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
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
  stm32_printf("Hello world!\r\n");
  SET_DUTY_RATE_SERVO_0(0.05);
  SET_DUTY_RATE_SERVO_1(0.05);
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

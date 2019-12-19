#include "cppmain.hpp"

#include "main.h"
#include "tim.h"
#include "gpio.h"
#include "usart.h"

#include "stm32_printf/stm32_printf.h"

#define CONTROL_LOOP_TIME 0.01 // sec
#define SET_DUTY_RATE_SERVO_0(duty_rate) (htim1.Instance->CCR1 = SERVO_PWM_PERIOD * duty_rate)
#define SET_DUTY_RATE_SERVO_1(duty_rate) (htim1.Instance->CCR2 = SERVO_PWM_PERIOD * duty_rate)
#define SERVO_0_DUTY_RATE_UP 0.045
#define SERVO_0_DUTY_RATE_DOWN 0.089
#define SERVO_1_DUTY_RATE_VERTICAL 0.104
#define SERVO_1_DUTY_RATE_HORIZONTAL 0.057
#define SOLENOID_ON GPIO_PIN_SET
#define SOLENOID_OFF GPIO_PIN_RESET

double servo_0_output = SERVO_0_DUTY_RATE_UP;
double servo_1_output = SERVO_1_DUTY_RATE_VERTICAL;
GPIO_PinState solenoid_output = SOLENOID_OFF;

int time_count = 0;

enum class Move {
  OPEN_CARD,
  CHANGE_TO_ATTACK_POSITION,
  CHANGE_TO_DEFENSE_POSITION,
  WAIT,
};
Move status = Move::WAIT;

bool move_open_card();
bool move_change_to_attack_position();
bool move_change_to_defense_position();

void setup(void)
{
  stm32_printf_init(&huart1);

  HAL_TIM_Base_Start_IT(&htim7);

  SET_DUTY_RATE_SERVO_0(servo_0_output);
  SET_DUTY_RATE_SERVO_1(servo_1_output);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
}

void loop(void)
{
  if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_RESET) {
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
  }
  else {
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
  }

  switch (status) {
    case Move::OPEN_CARD:
      if (move_open_card()) status = Move::WAIT;
      break;

    case Move::CHANGE_TO_ATTACK_POSITION:
      if (move_change_to_attack_position()) status = Move::WAIT;

      break;

    case Move::CHANGE_TO_DEFENSE_POSITION:
      if (move_change_to_defense_position()) status = Move::WAIT;

    case Move::WAIT:
      break;

    default:
      break;
  }

  if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3) == GPIO_PIN_SET) {
    if (status == Move::WAIT) {
      status = Move::OPEN_CARD;
    }
  }
  else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == GPIO_PIN_SET) {
    if (status == Move::WAIT) {
      status = Move::CHANGE_TO_ATTACK_POSITION;
    }
  }
  else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == GPIO_PIN_SET) {
    if (status == Move::WAIT) {
      status = Move::CHANGE_TO_DEFENSE_POSITION;
    }
  }

  SET_DUTY_RATE_SERVO_0(servo_0_output);
  SET_DUTY_RATE_SERVO_1(servo_1_output);
  HAL_GPIO_WritePin(SOLENOID_GPIO_Port, SOLENOID_Pin, solenoid_output);

  stm32_printf("%d", time_count);
  stm32_printf("\r\n");
}

//**************************
//    タイマ割り込み関数
//**************************
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  // TIM7 100msecタイマ
  if (htim->Instance == TIM7) {
    if (time_count <= 0) {
      time_count = 0;
    }
    else {
      time_count--;
    }
  }
}

bool move_open_card()
{
  static enum {
    ARM_DOWN_WITHOUT_CARD,
    ATTRACT_CARD,
    ARM_UP_WITH_CARD,
    WAIT_FEW_SECOND,
    ARM_DOWN_WITH_CARD,
    DISTRACT_CARD,
    ARM_UP_WITHOUT_CARD,
  } status = ARM_DOWN_WITHOUT_CARD,
    following_status = ARM_UP_WITHOUT_CARD;

  switch (status) {
    case ARM_DOWN_WITHOUT_CARD:
      if (status != following_status) {
        time_count = 5;
        following_status = status;
      }
      servo_0_output = SERVO_0_DUTY_RATE_DOWN;
      stm32_printf("  ARM_DOWN_WITHOUT_CARD  ");
      if (time_count == 0) status = ATTRACT_CARD;
      break;

    case ATTRACT_CARD:
      if (status != following_status) {
        time_count = 2;
        following_status = status;
      }
      solenoid_output = SOLENOID_ON;
      stm32_printf("  ATTRACT_CARD  ");
      if (time_count == 0) status = ARM_UP_WITH_CARD;
      break;

    case ARM_UP_WITH_CARD:
      if (status != following_status) {
        time_count = 5;
        following_status = status;
      }
      servo_0_output = SERVO_0_DUTY_RATE_UP;
      stm32_printf("  ARM_UP_WITH_CARD  ");
      if (time_count == 0) status = WAIT_FEW_SECOND;
      break;

    case WAIT_FEW_SECOND:
      if (status != following_status) {
        time_count = 50;
        following_status = status;
      }
      stm32_printf("  WAIT_FEW_SECOND  ");
      if (time_count == 0) status = ARM_DOWN_WITH_CARD;
      break;

    case ARM_DOWN_WITH_CARD:
      if (status != following_status) {
        time_count = 5;
        following_status = status;
      }
      servo_0_output = SERVO_0_DUTY_RATE_DOWN;
      stm32_printf("  ARM_DOWN_WITH_CARD  ");
      if (time_count == 0) status = DISTRACT_CARD;
      break;

    case DISTRACT_CARD:
      if (status != following_status) {
        time_count = 2;
        following_status = status;
      }
      solenoid_output = SOLENOID_OFF;
      stm32_printf("  DISTRACT_CARD  ");
      if (time_count == 0) status = ARM_UP_WITHOUT_CARD;
      break;

    case ARM_UP_WITHOUT_CARD:
      if (status != following_status) {
        time_count = 5;
        following_status = status;
      }
      servo_0_output = SERVO_0_DUTY_RATE_UP;
      stm32_printf("  ARM_UP_WITHOUT_CARD  ");
      if (time_count == 0) {
        status = ARM_DOWN_WITHOUT_CARD;
        return true;
      }
      break;

    default:
      break;
  }
  return false;
}

bool move_change_to_attack_position()
{
  return true;
}

bool move_change_to_defense_position()
{
  return true;
}

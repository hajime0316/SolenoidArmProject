/* 
 * File:    stm32_servo.cpp
 * version: 1.0.0
 * Author:  Hajime
 *
 * Created on 2019/12/17
 */

#include "stm32_servo.hpp"

Stm32Servo::Stm32Servo(TIM_HandleTypeDef *htim, uint32_t channel, Config config)
{
  htim_ = htim;
  channel_ = channel;
  config_ = config;

  set_ccr_register(htim_, channel_, config_.counter_period * config_.min_duty_rate);

  HAL_TIM_PWM_Start(htim, channel_);
}

Stm32Servo::~Stm32Servo()
{
}

double Stm32Servo::add_angle(double angle)
{
  double new_angle = angle_ + angle;

  return set_angle(new_angle);
}

double Stm32Servo::set_angle(double angle)
{
    if (angle < config_.min_angle) {
    angle_ = config_.min_angle;
  }
  else if (angle > config_.max_angle) {
    angle_ = config_.max_angle;
  }
  else {
    angle_ = angle;
  }

  double duty_rate = calc_duty_rate_from_angle(angle_);
  set_ccr_register(htim_, channel_, config_.counter_period * duty_rate);

  return angle_;
}

double Stm32Servo::get_angle() const
{
  return angle_;
}

void Stm32Servo::set_ccr_register(TIM_HandleTypeDef *htim, uint32_t channel, uint32_t pulse)
{
  switch (channel) {
    case TIM_CHANNEL_1:
      htim->Instance->CCR1 = pulse;
      break;

    case TIM_CHANNEL_2:
      htim->Instance->CCR2 = pulse;
      break;

    case TIM_CHANNEL_3:
      htim->Instance->CCR3 = pulse;
      break;

    case TIM_CHANNEL_4:
      htim->Instance->CCR4 = pulse;
      break;

    default:
      break;
  }

  return;
}

double Stm32Servo::calc_duty_rate_from_angle(double angle)
{
  return config_.min_duty_rate +
         (config_.max_duty_rate - config_.min_duty_rate) /
             (config_.max_angle - config_.min_angle) *
             (angle - config_.min_angle);
}

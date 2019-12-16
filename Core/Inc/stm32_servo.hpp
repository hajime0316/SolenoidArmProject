/* 
 * File:    stm32_servo.hpp
 * version: 1.0.0
 * Author:  Hajime
 *
 * Created on 2019/12/17
 */

#ifndef STM32_SERVO_HPP_
#define STM32_SERVO_HPP_

#include "main.h"

class Stm32Servo {
  struct Config {
    uint32_t counter_period;
    double max_duty_rate;
    double max_angle;
    double min_duty_rate;
    double min_angle;
  };

 private:
  TIM_HandleTypeDef *htim_;
  uint32_t channel_;
  Config config_;
  double angle_;

 public:
  Stm32Servo(TIM_HandleTypeDef *, uint32_t, Config);
  virtual ~Stm32Servo();
  double add_angle(double);
  void set_angle(double);
  double get_angle() const;

 private: // privateなメソッド
  void set_ccr_register(TIM_HandleTypeDef *, uint32_t, uint32_t);
  double calc_duty_rate_from_angle(double);
};

#endif // STM32_SERVO_HPP_

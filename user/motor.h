#ifndef __MOTOR_H___
#define __MOTOR_H___


#include "main.h"
#include "pid.h"


#define  MOTOR_PWM_FREQ   17000   //PWM���Ƶ��
#define  MOTOR_PWM_LIMIT  6500    //PWM����޷���65%


void motor_init(void);
void set_pwm_left(int32_t duty);
void set_pwm_right(int32_t duty);


#endif
/*
 * SubSys_AlertControl_Driver.h
 *
 *  Created on: Jul 19, 2024
 *      Author: yunus
 */

#ifndef SAT_CARRIER_SUBSYS_DRIVERS_SUBSYS_INC_SUBSYS_ALERTCONTROL_DRIVER_H_
#define SAT_CARRIER_SUBSYS_DRIVERS_SUBSYS_INC_SUBSYS_ALERTCONTROL_DRIVER_H_
#define SAT_CARRIER_SUBSYS_DRIVER_ALERTCONTROL_H


#include "main.h"

/**
 * @func  : void Buzz_ON(TIM_HandleTypeDef *htim_X, uint32_t PWM_Channel_X)
 * @brief : Buzzer is set when the function is called(DutyCycle is configurated by the CCRx REG)
 * @param :  htim_X        : it's set by &htimX. X is the number of timers
 * @param :  PWM_Channel_X :  it's set by TIM_CHANNEL_X . X is the number of timer's channels
 * @retval: None
 */
void PassiveBuzz_ON(TIM_HandleTypeDef *htim_X, uint32_t PWM_Channel_X);

/**
 * @func.  : void PassiveBuzz_OFF(TIM_HandleTypeDef *htim_X, uint32_t PWM_Channel_X)
 * @brief  : Buzzer is deactivated when the function is called (DutyCycle is zero)
 * @param  : htim_X        : it's set by &htimX. X is the number of timers
 * @param  : PWM_Channel_X :  it's set by TIM_CHANNEL_X . X is the number of timer's channels
 * @retval : None
 */
void PassiveBuzz_OFF(TIM_HandleTypeDef *htim_X, uint32_t PWM_Channel_X);

/**
 * @func.  : void PassiveBuzz_Init(TIM_HandleTypeDef *htim_X, uint32_t PWM_Channel_X)
 * @brief  : Buzzer and timer's settings is prepared when the function is called
 * @param  : htim_X        : it's set by &htimX. X is the number of timers
 * @param  : PWM_Channel_X :  it's set by TIM_CHANNEL_X . X is the number of timer's channels
 * @retval : None
 */
void PassiveBuzz_Init(TIM_HandleTypeDef *htim_X, uint32_t PWM_Channel_X);

#endif /* SAT_CARRIER_SUBSYS_DRIVERS_SUBSYS_INC_SUBSYS_ALERTCONTROL_DRIVER_H_ */

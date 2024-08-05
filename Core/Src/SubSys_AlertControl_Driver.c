
#include "SubSys_AlertControl_Driver.h"



/**
 * @func  : void Buzz_ON(TIM_HandleTypeDef *htim_X, uint32_t PWM_Channel_X)
 * @brief : Buzzer is set when the function is called(DutyCycle is configurated by the CCRx REG)
 * @param :  htim_X        : it's set by &htimX. X is the number of timers
 * @param :  PWM_Channel_X :  it's set by TIM_CHANNEL_X . X is the number of timer's channels
 * @retval: None
 */
void PassiveBuzz_ON(TIM_HandleTypeDef *htim_X, uint32_t PWM_Channel_X){

	switch (PWM_Channel_X){

	case TIM_CHANNEL_1 :
		htim_X->Instance->CCR1 =  0.5 * (htim_X->Instance->ARR);
	break;

	case TIM_CHANNEL_2 :
		htim_X->Instance->CCR2 =  0.5 * (htim_X->Instance->ARR);
	break;

	case TIM_CHANNEL_3 :
		htim_X->Instance->CCR3 =  0.5 * (htim_X->Instance->ARR);
	break;

	case TIM_CHANNEL_4 :
		htim_X->Instance->CCR4 =  0.5 * (htim_X->Instance->ARR);
	break;
	}


}



/**
 * @func.  : void PassiveBuzz_OFF(TIM_HandleTypeDef *htim_X, uint32_t PWM_Channel_X)
 * @brief  : Buzzer is deactivated when the function is called (DutyCycle is zero)
 * @param  : htim_X        : it's set by &htimX. X is the number of timers
 * @param  : PWM_Channel_X :  it's set by TIM_CHANNEL_X . X is the number of timer's channels
 * @retval : None
 */
void PassiveBuzz_OFF(TIM_HandleTypeDef *htim_X, uint32_t PWM_Channel_X){
	switch (PWM_Channel_X){

	case TIM_CHANNEL_1 :
		htim_X->Instance->CCR1 = 0;
	break;

	case TIM_CHANNEL_2 :
		htim_X->Instance->CCR2 = 0;
	break;

	case TIM_CHANNEL_3 :
		htim_X->Instance->CCR3 = 0;
	break;

	case TIM_CHANNEL_4 :
		htim_X->Instance->CCR4 = 0;
	break;
	}
}

/**
 * @func.  : void PassiveBuzz_Init(TIM_HandleTypeDef *htim_X, uint32_t PWM_Channel_X)
 * @brief  : Buzzer and timer's settings is prepared when the function is called
 * @param  : htim_X        : it's set by &htimX. X is the number of timers
 * @param  : PWM_Channel_X :  it's set by TIM_CHANNEL_X . X is the number of timer's channels
 * @param  : Buzzer_Frequency : it's necessary to calculate PSC value, it is recommended to look at product datasheet
 * @retval : None
 */
void PassiveBuzz_Init(TIM_HandleTypeDef *htim_X, uint32_t PWM_Channel_X){

	switch (PWM_Channel_X){
	case TIM_CHANNEL_1 :
		   htim_X->Instance->CCR1 = 0;
	break;

	case TIM_CHANNEL_2 :
		   htim_X->Instance->CCR2 = 0;
	break;

	case TIM_CHANNEL_3 :
		   htim_X->Instance->CCR3 = 0;
	break;

	case TIM_CHANNEL_4 :
		   htim_X->Instance->CCR4 = 0;
	break;
	}

	HAL_TIM_PWM_Start(htim_X, PWM_Channel_X);

	PassiveBuzz_ON(htim_X, PWM_Channel_X);
	HAL_Delay(1000);
	PassiveBuzz_OFF(htim_X, PWM_Channel_X);
}

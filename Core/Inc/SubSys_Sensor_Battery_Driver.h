/*
 * SubSys_Sensor_Battery_Driver.h
 *
 *  Created on: Jul 19, 2024
 *      Author: yunus
 */

#ifndef SAT_CARRIER_SUBSYS_DRIVERS_SUBSYS_INC_SUBSYS_SENSOR_BATTERY_DRIVER_H_
#define SAT_CARRIER_SUBSYS_DRIVERS_SUBSYS_INC_SUBSYS_SENSOR_BATTERY_DRIVER_H_
#define SAT_CARRIER_SUBSYS_DRIVERS_SENSOR_BATTERY_H

/******************************************************************************
         			#### INCLUDES ####
******************************************************************************/

#include "main.h"
#include "stdio.h"

/******************************************************************************
         			#### DEFINITIONS ####
******************************************************************************/
#define ConstantOfReferanceVoltage 3.30
#define ResolationValueOfBits 4095
/******************************************************************************
         			#### EXTERNAL VARIABLES ####
******************************************************************************/
extern float BatteryVoltage;
extern uint8_t NumSerialBat;

/******************************************************************************
         			#### FUNCTIONS ####
******************************************************************************/
void  MeasBattery_Init(int NumSerialBat);
float ReadBatteryVoltage(ADC_HandleTypeDef *hadc);

#endif /* SAT_CARRIER_SUBSYS_DRIVERS_SUBSYS_INC_SUBSYS_SENSOR_BATTERY_DRIVER_H_ */

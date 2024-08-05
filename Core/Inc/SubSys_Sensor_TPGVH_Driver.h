/*
 * SubSys_Sensor_TPGVH_Driver.h
 *
 *  Created on: Jul 19, 2024
 *      Author: yunus
 */

#ifndef SAT_CARRIER_SUBSYS_DRIVERS_SUBSYS_INC_SUBSYS_SENSOR_TPGVH_DRIVER_H_
#define SAT_CARRIER_SUBSYS_DRIVERS_SUBSYS_INC_SUBSYS_SENSOR_TPGVH_DRIVER_H_
#define SAT_CARRIER_SUBSYS_DRIVERS_SENSOR_TPGVH_H
/******************************************************************************
         			#### MS5611 INCLUDES ####
******************************************************************************/
#include "main.h"
#include "stdbool.h"
#include "stdint.h"
#include "math.h"

/******************************************************************************
         			#### MS5611 EXTERNAL VARIABLES ####
******************************************************************************/

extern I2C_HandleTypeDef hi2c1;
extern float  MS5611_Press;		/*! Pressure data variable 			*/
extern float  MS5611_Temp;		/*! Temperature data variable 		*/
extern float  MS5611_Altitude;	/*! Vertical Altitude data variable */
extern float  MS5611_VertSpeed; /*! Vertical Speed data variable    */
extern float  MS5611_VertAcc;	/*! Vertical Acceleration variable  */
extern float  MS5611_gForce;	/*! Vertical g force data variable  */
extern float  SatCar_Mass;		/*! Total mass of Satellites Carrier module */

/*! These macros provide to calculate the altitude of MS5611 */
/*! The geographical altitude can be calculated by determining the absolute pressure.*/
#define SeaLevelPress  101325
#define SeaLevelTemp   288.15
#define GradientTemp   0.0065
#define GravityAccel   9.8062
#define GasCoefficient 287.052

/******************************************************************************
         				#### MS5611 ENUMS ####
******************************************************************************/

typedef enum
{

  MS5611_OK   	= 0,
  MS5611_ERROR  = 1

}MS5611_StatusTypeDef;

/******************************************************************************
         				#### MS5611 STRUCTURES ####
******************************************************************************/

typedef struct{

	uint16_t C1; 	/*! Pressure sensitivity 							(SENSt1)   */
	uint16_t C2;	/*! Pressure offset 								(OFFt1)    */
	uint16_t C3;	/*! Temperature coefficient of pressure sensitivity (TCS) 	   */
	uint16_t C4;	/*! Temperature coefficient of pressure offset 		(TCO) 	   */
	uint16_t C5;	/*! Reference temperature 20 Â°C 					(Tref) 	   */
	uint16_t C6;	/*! Temperature coefficient of the temperature 		(TEMPSENS) */

	uint16_t crc;	/*! 4-bit CRC has been implemented to check the data validity in memory*/

}MS5611_CalibrationCoef_TypeDef;

typedef struct{

	uint32_t D1;		/*! Digital raw pressure value */
	uint32_t D2;		/*! Digital raw temperature value */
	float dT;			/*! Difference between actual and reference temperature */
	float TEMP;			/*! Actual temperature (-40â€¦85Â°C with 0.01Â°C resolution) */
	float P;			/*! Compensated pressure (10â€¦1200mbar with 0.01mbar resolution) */
	float OFF;			/*! Offset at actual temperature */
	float SENS;			/*! Sensitivity at actual temperature */
	float OFF2;			/*! Offset at actual temperature_2 */
	float SENS2;  		/*! Sensitivity at actual temperature_2 */
	float TEMP2; 		/*! Actual temperature_2 (-40â€¦<20Â°C with 0.01Â°C resolution) */

}MS5611_CalculationParams_TypeDef;

/**
 * @brief  Calculate the speed, acceleration and gForce by using these variables
 *
 */
typedef struct{

	float alt0;
	float alt1;
	float holdAlt;

	float acc0;
	float acc1;
	float holdAcc;

	float spd0;
	float spd1;
	float holdSpd;

}MS5611_DeltaData;

typedef struct{

	I2C_HandleTypeDef *i2c;

	uint16_t I2C_ADDRESS;

	MS5611_CalibrationCoef_TypeDef Clb_Cf;
	MS5611_CalculationParams_TypeDef ClcPrms;
	MS5611_DeltaData DeltaData;
/*	float SeaLevelPress;			! SeaLevel Press is an input that user have to write city pressure from the weather programs
	 	 	 	 	 	 	 	 	 *	@arg : 101.140 Pascal is for TÃ¼rkiye(country) / Ankara(city) / KeÃ§iÃ¶ren(town)

	float SeaLevelTemp;				 SeLevel Temp is an input that user have to write city temperature from the weather programs
									 *  @arg : 208 Kelvin is for TÃ¼rkiye(country) / Ankara(city) / KeÃ§iÃ¶ren(town)
									 */

	float FixedAltitude;			/*! It gets otomaticly zero or calculated sea level pressure after selecting Ref_Alt_Sel */
	char  Ref_Alt_Sel;				/**
		   	   	   	   	   	   	   	 * Ref_Alt_Sel is a selection;  For 'm' : it sets the reference altitude to the current location (0 meters)
		   	   	   	   	   	   	   	 * 							    For 'M' : it sets the reference altitude to sea level
		   	   	   	   	   	   	   	 */
}MS5611_HandleTypeDef;


/******************************************************************************
         			#### MS5611 PROTOTYPES OF FUNCTIONS ####
******************************************************************************/

/**
  * @brief  MS5611 sensor initialization.
  * @param  dev_MS5611 general handle.
  * @retval booleans.
  */
MS5611_StatusTypeDef MS5611_Init(MS5611_HandleTypeDef *dev);


/**
  * @brief
  * @param  dev_MS5611 general handle.
  * @retval MS5611 Status.
  */
void MS5611_Read_ActVal(MS5611_HandleTypeDef *dev);


/**
  * @brief  Retrieves calibration coefficient data(from C1 to C6) from the MS5611 chip and stores them in CalibDatas.
  * @param  dev_MS5611 general handle.
  * @retval MS5611 Status.
  */
void MS5611_Get_CalibCoeff(MS5611_HandleTypeDef *dev);


/**
  * @brief  MS5611 are reset and if it's not completed successfully the sensor is initialized one more time
  * @param  dev_MS5611 general handle.
  * @retval MS5611 Status.
  */
void MS5611_Reset(MS5611_HandleTypeDef *dev);


/**
  * @brief  ADC Registers will be reading for raw pressure and temperature values
  * @param  dev_MS5611 general handle.
  * @retval MS5611 Status.
  */
void MS5611_ReadRaw_Press_Temp(MS5611_HandleTypeDef *dev);


/**
  * @brief  Calculate 1st order temperature and pressure  according to MS5611 1st order algorithm
  * @param  dev_MS5611 general handle.
  */
void MS5611_FirstCalculateDatas(MS5611_HandleTypeDef *dev);


/**
  * @brief  If it's needed, Calculate 2st order temperature and pressure  according to MS5611 2st order algorithm
  * @param  dev_MS5611 general handle.
  */
void MS5611_SecondCalculateDatas(MS5611_HandleTypeDef *dev);


/**
  * @brief  Vertical Altitude is calculated by using pressure and some coefficients
  * @param  dev_MS5611 general handle.
  * @retval Vertical Altitude value
  */
float MS5611_Calc_Altitude(MS5611_HandleTypeDef *dev);

float MS5611_Calc_TemporaryAltitude(MS5611_HandleTypeDef *dev);

void MS5611_ResetRef_DeltaVal(MS5611_HandleTypeDef *dev);


void MS5611_Calc_VertSpd(MS5611_HandleTypeDef *dev, float MS5611_Altitude, float *MS5611_VertSpeed);
void MS5611_Calc_VertAcc(MS5611_HandleTypeDef *dev, float MS5611_VertSpeed,float *MS5611_VertAcc);
void MS5611_Calc_gForce(MS5611_HandleTypeDef *dev, float *MS5611_gForce, float SatCar_Mass, float MS5611_VertAcc);

#endif /* SAT_CARRIER_SUBSYS_DRIVERS_SUBSYS_INC_SUBSYS_SENSOR_TPGVH_DRIVER_H_ */

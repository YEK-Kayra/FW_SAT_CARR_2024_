/*
 * SubSys_SDcard_Driver.h
 *
 *  Created on: Jul 19, 2024
 *      Author: yunus
 */

#ifndef SAT_CARRIER_SUBSYS_DRIVERS_SUBSYS_INC_SUBSYS_SDCARD_DRIVER_H_
#define SAT_CARRIER_SUBSYS_DRIVERS_SUBSYS_INC_SUBSYS_SDCARD_DRIVER_H_
#define SAT_CARRIER_SUBSYS_DRIVERS_SDCARD_H

/******************************************************************************
         			#### SD CARD OPERATIONS INCLUDES ####
******************************************************************************/
#include "main.h"
#include "fatfs.h"
#include "stm32f4xx_hal.h"
/*To convert number value to string format*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


/******************************************************************************
         			#### SD CARD OPERATIONS DEFINITIONS ####
******************************************************************************/
#define LineSize 120 /*SD_Datas_HandleTypeDef struct's total byte*/


/******************************************************************************
         			#### SD CARD OPERATIONS STRUCTURES ####
******************************************************************************/
typedef struct{

float Carr_Pressure;			/* Unit : pascal 		(11Byte) e.g => "101325.12" */
float Carr_VertSpeed;			/* Unit : meter/second	(6Byte) e.g  => "200.12" 	*/
float Carr_VertHeight;			/* Unit : meter 		(4Byte) e.g  => "223.4"		*/
float Carr_Temperature;			/* Unit : °celcius 		(5Byte) e.g  => "32.78"		*/

float Carr_Voltage;				/* Unit : Voltage 		(4Byte) e.g => "8.42"		*/

float Carr_GPS_Latitude;		/* Unit : meter		  (10Byte) e.g => "57.912109" -90° to +90°      */
float Carr_GPS_Longitude;		/* Unit : degrees (°) (10Byte) e.g => "78.0203478" -90° to +90°	    */
float Carr_GPS_Altitude;		/* Unit : degrees (°) (10Byte) e.g => "30.1585941" 0m to 9000meter  */

float Carr_gForce;				/* Unit : newton(F)   (5Byte)  e.g => "49.76Newton"					*/

uint16_t Carr_PacketNO;		    /* Unit : Number (2Byte) e.g => "1265"  							*/

}SD_Datas_HandleTypeDef;


/******************************************************************************
         			#### SD CARD OPERATIONS FUNCTIONS ####
******************************************************************************/

/**
 * @brief : FatFs requires work area (filesystem object) for each logical drives (FAT volumes). Prior to perform any file/directory operations,
 * 			a filesystem object needs to be registered with f_mount function for the logical drive.
 *
 * @param[IN]  const TCHAR* SD_path : Can not write "E:" , "e:",  "e\". "E/" is ok for FR_OK
 * @param[IN]  BYTE Mount_Op : if you write "0" is that , if you write "1" is that
 *
 * @return FRESULT
 *
 */
FRESULT SD_Mount(const TCHAR* SD_path,BYTE Mount_Op);


/**
 * @brief : This function creates a new directory
 *
 * @param[IN] const TCHAR* SD_Dir 	   : We create a file(directory) in the SD card
 * @param[IN] const TCHAR* SD_FileName : We create a x.TXT file. x will be filled by your given name @arg sub1, KLM, DIR
 * @param[IN] char* SD_Buffer 		   : We create a buffer that contains satellite's carrier variables. We fill it by SD_Data objects variables
 *
 * @return FRESULT
 *
 */
FRESULT SD_Create_Dir_File(const TCHAR* SD_Dir,const TCHAR* SD_FileName,char* SD_Buffer);


/**
 * @brief : struct variable of SD_Datas_HandleTypeDef that contains "SD_Data"s variable like SD_Data.Carr_Pressure,SD_Data.Carr_Temperature and more.
 * 			the parameters will be update for each loop and convert to string type.
 *
 */
void SD_FillVariables(void);


/**
 * @brief : Sd_Buffer is written into the SD card.
 *
 * @param[IN] char* SD_Buffer          : We create a buffer that contains satellite's carrier variables. We fill it by SD_Data objects variables
 * @param[IN] const TCHAR* SD_FileName : We create a x.TXT file. x will be filled by your given name
 */
FRESULT SD_Write(char* SD_Buffer,const TCHAR* SD_FileName);

#endif /* SAT_CARRIER_SUBSYS_DRIVERS_SUBSYS_INC_SUBSYS_SDCARD_DRIVER_H_ */

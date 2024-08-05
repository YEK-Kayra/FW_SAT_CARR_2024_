/******************************************************************************
         			#### SD CARD OPERATIONS INCLUDES ####
******************************************************************************/
#include "SubSys_SDcard_Driver.h"
#include "stm32f4xx_hal.h"


/******************************************************************************
         			#### SD CARD OPERATIONS STRUCTURES ####
******************************************************************************/
/* LIBRARY VARIABLES OF "ff.h" AND "FatFs.h" */
FATFS FATFS_Ob; 	 /* File system object */
FIL FilePage;   	 /* SD card page that is stored in the directory */
FRESULT SD_result;   /* File function return code */

/* SD CARD TOP FRAME */
char* DataTopFrame = "CAR_PacketNo, CAR_Pressure, CAR_Temperature, CAR_VertHeight, CAR_VertSpeed, CAR_gForce, CAR_Latitude, CAR_Longitude, CAR_Altitude, CAR_Voltage ";

/* We create an object that keeps different satellite variable */
SD_Datas_HandleTypeDef SD_Data;

//We create a buffer that contains satellite's carrier variables. We fill it by SD_Data objects variables
char* SdDatasBuf[LineSize];

extern I2C_HandleTypeDef hi2c1;
extern float  MS5611_Press;		/*! Pressure data variable 			*/
extern float  MS5611_Temp;		/*! Temperature data variable 		*/
extern float  MS5611_Altitude;	/*! Vertical Altitude data variable */
extern float  MS5611_VertSpeed; /*! Vertical Speed data variable    */
extern float  MS5611_VertAcc;	/*! Vertical Acceleration variable  */
extern float  MS5611_gForce;	/*! Vertical g force data variable  */
extern float  SatCar_Mass;		/*! Total mass of Satellites Carrier module */

extern float GPS_Altitude;				/*! Vertical distance info of satellite beetween */
extern float GPS_Longitude;				/*! Location info of satellite on the earth 	 */
extern float GPS_Latitude;				/*! Location info of satellite on the earth 	 */

extern float BatteryVoltage;


void SD_FillVariables(void){

    	SD_Data.Carr_Pressure 	 = MS5611_Press;  // there will be "MS5611_Press" instead of "101325.12"
    	SD_Data.Carr_Temperature = MS5611_Temp;
    	SD_Data.Carr_VertHeight  = MS5611_Altitude;
    	SD_Data.Carr_VertSpeed 	 =  MS5611_VertSpeed;

    	SD_Data.Carr_GPS_Latitude  = GPS_Latitude;
    	SD_Data.Carr_GPS_Longitude = GPS_Longitude;
    	SD_Data.Carr_GPS_Altitude  = GPS_Altitude;

    	SD_Data.Carr_gForce		   = MS5611_gForce;
    	SD_Data.Carr_Voltage   	   = BatteryVoltage;
    	SD_Data.Carr_PacketNO 	  += 1;

    	sprintf(SdDatasBuf,"<%d>, <%.2f>, <%.2f>, <%.2f>, <%.2f>, <%.2f>, <%.2f>, <%.2f>, <%.2f>, <%.2f>\n",
																											 SD_Data.Carr_PacketNO     ,SD_Data.Carr_Pressure	,
																											 SD_Data.Carr_Temperature  ,SD_Data.Carr_VertHeight  ,
																											 SD_Data.Carr_VertSpeed    ,SD_Data.Carr_gForce		,
																											 SD_Data.Carr_GPS_Latitude ,SD_Data.Carr_GPS_Longitude,
																											 SD_Data.Carr_GPS_Altitude ,SD_Data.Carr_Voltage);




}


FRESULT SD_Mount (const TCHAR* SD_path, BYTE Mount_Op)
{

	SD_result = f_mount(&FATFS_Ob, SD_path, Mount_Op);

	if(SD_result != FR_OK){
		/**
		 * Buzzer will be activated like biiip biip bip
		 */
		//while(1);
	}
	else{

		return FR_OK;

	}
}


FRESULT SD_Create_Dir_File(const TCHAR* SD_Dir,const TCHAR* SD_FileName,char* SD_Buffer){

	SD_result = f_mkdir(SD_Dir);

	if((SD_result != FR_OK)&&(SD_result != FR_EXIST)){
		/**
		 * Buzzer will be activated like biiip biip bip
		 */
		while(1);
	}
	else{

		SD_result = f_open(&FilePage, SD_FileName, FA_CREATE_ALWAYS | FA_WRITE);
		SD_result = f_close(&FilePage);

		sprintf(SD_Buffer,"%s\n",DataTopFrame);
		SD_Write(SD_Buffer,"SAT_CAR/STM32.TXT");

		if(SD_result != FR_OK){
				/**
				 * Send to ground station error message
			     */
				while(1);
			}
		SD_result = f_close(&FilePage); //invalid object hatası verdi
		return FR_OK;
	}

}


FRESULT SD_Write(char* SD_Buffer,const TCHAR* SD_FileName){

	UINT written;

	SD_result = f_open(&FilePage, SD_FileName, FA_OPEN_APPEND | FA_WRITE);

	SD_result =  f_write(&FilePage,SD_Buffer,strlen(SD_Buffer),&written);

	SD_result = f_close(&FilePage);

	return FR_OK;
}

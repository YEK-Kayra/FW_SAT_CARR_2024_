
#ifndef SAT_CARRIER_SUBSYS_DRIVERS_SUBSYS_INC_SUBSYS_WIRELESSCOMMUNICATION_TELEMETRY_DRIVER_H_
#define SAT_CARRIER_SUBSYS_DRIVERS_SUBSYS_INC_SUBSYS_WIRELESSCOMMUNICATION_TELEMETRY_DRIVER_H_
#define SAT_CARRIER_SUBSYS_DRIVERS_WIRELESSCOMMUNICATION_TELEMETRY_H

/******************************************************************************
         				#### WIRELESSCOM INCLUDES ####
******************************************************************************/
#include "main.h"

/******************************************************************************
         				#### WIRELESSCOM DEFINITIONS ####
******************************************************************************/

/*! 200 is the number of packets the LoRa module can send in a single transmission */
#define SizeOf_Wireless_TX_Buff_Carrier 	200

/******************************************************************************
         				#### WIRELESSCOM ENUMS ####
******************************************************************************/
typedef enum{

	Sat_Carrier = 1,
	Sat_Payload,
	GroundStation

}MissionUnit;


/******************************************************************************
         				#### WIRELESSCOM EXTERNS ####
******************************************************************************/

extern float MS5611_Press;            /*! Pressure data variable 			*/
extern float MS5611_Temp;             /*! Temperature data variable 		*/
extern float MS5611_Altitude;         /*! Vertical Altitude data variable 	*/
extern float MS5611_VertSpeed;        /*! Vertical Speed data variable 		*/
extern float MS5611_VertAcc;          /*! Vertical Acceleration variable 	*/
extern float MS5611_gForce;           /*! Vertical g force data variable 	*/
extern float SatCar_Mass;             /*! Total mass of Satellites Carrier module */

extern float BatteryVoltage;		  /*! Voltage of the Satellite's battery  	  */

extern float GPS_Altitude;			  /*! Vertical distance info of satellite beetween  */
extern float GPS_Longitude;			  /*! Location info of satellite on the earth 	 	*/
extern float GPS_Latitude;			  /*! Location info of satellite on the earth 	 	*/


/******************************************************************************
         				#### WIRELESSCOM STRUCTS ####
******************************************************************************/

typedef struct{

	/*####################### SATELLITE CARRIER UNIT VARIABLES #######################*/

	  	 /*-----------------------THERE ARE 59 BYTE FOR CARRIER PACKET--------------------------*/
			/*! Will be filled by MS5611 sensor*/
			float Carr_Pressure;		/* Unit : pascal 		(12Byte)e.g  => "101325.5938"   			*/
			float Carr_VertSpeed;		/* Unit : meter/second	(5Byte) e.g  => "1.0, 10.2, 158.7 "		  	*/
			float Carr_VertHeight;		/* Unit : meter 		(6Byte) e.g  => "223.4, 1023.7"		  		*/
			float Carr_Temperature;		/* Unit : C°,celcius 	(4Byte) e.g  => "32.7"		  				*/

			/* Will be filled by used ADC interface*/
			float Carr_BatteryVoltage;	/* Unit : Voltage		(4Byte) e.g => "8.25"						*/

			/* Will be filled by L-86 GPS*/
			float Carr_GPS_Latitude;	/* Unit : degrees and minute(°)(')	(10Byte) e.g => "3150.7822N"  ,	NMEA data it means 31° 50.7822' minute 	'N' will be send separately		 */
			float Carr_GPS_Longitude;	/* Unit : degrees and minute(°)(') 	(10Byte) e.g => "11711.9278E" , NMEA data it means 117° 11.9278 minute	'E' will be send separately		 */
			float Carr_GPS_Altitude;	/* Unit : meter 					(6Byte) e.g =>  "1214.4"	  , NMEA data it means 1214.4 meter from sea. GPGGA is provides 			 */
			uint32_t NumOfPacket;					/* Unit : unsigned int (4Byte) e.g => "337	  */

}SubSys_WirelessCom_VariableTypeDef;

typedef struct{

	uint8_t Tx[SizeOf_Wireless_TX_Buff_Carrier];	 /*! Buffer for Datas that send to Lora*/
	char 	Temp[SizeOf_Wireless_TX_Buff_Carrier];
}SubSys_WirelessCom_BufferTypeDef;

typedef struct{

	/*! Inner structs */
	SubSys_WirelessCom_BufferTypeDef Buffer;
	SubSys_WirelessCom_VariableTypeDef	Variable;

	/*! Used device DMA and Usart interface settings */
	UART_HandleTypeDef *huartX;
	DMA_HandleTypeDef *hdma_usartX_rx;
	DMA_HandleTypeDef *hdma_usartX_tx;

	/*! Target device Address and Channel settings */
	uint8_t Target_ADDH;
	uint8_t Target_ADDL;
	uint8_t Target_Ch;

}SubSys_WirelessCom_APP_HandleTypeDef;


/******************************************************************************
         			#### WIRELESSCOM PROTOTYPES OF FUNCTIONS ####
******************************************************************************/

/**
  * @brief Decimal, float and other formats are converted as character and save them into the TX buffer.
  * 		When TX buffer is fulfilled , it is sent by UART interface.
  * @note  Follow the transmitting rules, each if and else has a reason
  * @param MissionUnit From_X, Packet type used for your specific purpose. Where do you  want to take it, select that.
  * 																	  @arg 0 : Sat_Carrier
  * 																 	  @arg 1 : Sat_Payload
  * 																  	  @arg 2 : Ground_Sation
  *
  * @param MissionUnit To_Y, Packet type used for your specific purpose. Where do you want to send it, select that.
  * 																	  @arg 0 : Sat_Carrier
  * 																 	  @arg 1 : Sat_Payload
  * 																  	  @arg 2 : Ground_Sation
  * @param SubSys_WirelessCom_APP_HandleTypeDef *dev_WirelessComApp , created object for wireless communication
  * @retval NONE
  */
void SubSys_WirelessCom_Telemetry_Transfer_From_To(MissionUnit From_X, MissionUnit To_Y, SubSys_WirelessCom_APP_HandleTypeDef *dev_WirelessComApp);



/**
 * @brief  : Creates 3 types packet for Carrier, Payload and Ground Station
 * @note   : Where do you want to send the packet, select that, For example you are a courier and
 * 			 carry the packet to the selected home(MissionUnite x)
 *
 * @param  : MissionUnit x, Packet type used for your specific purpose @arg 0 : Sat_Carrier
 * 																	   @arg 1 : Sat_Payload
 * 																	   @arg 2 : Ground_Sation
 *
 * @param  : SubSys_WirelessCom_APP_HandleTypeDef *dev_WirelessComApp , created object for wireless communication
 * @retval NONE
 */
void SubSys_WirelessCom_Telemetry_Create_Packet_For(MissionUnit x,SubSys_WirelessCom_APP_HandleTypeDef *dev_WirelessComApp);


#endif /* SAT_CARRIER_SUBSYS_DRIVERS_SUBSYS_INC_SUBSYS_WIRELESSCOMMUNICATION_TELEMETRY_DRIVER_H_ */

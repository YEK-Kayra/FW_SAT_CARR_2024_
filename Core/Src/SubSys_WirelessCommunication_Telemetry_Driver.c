
/******************************************************************************
         				#### WIRELESSCOM INCLUDES ####
******************************************************************************/
#include "SubSys_WirelessCommunication_Telemetry_Driver.h"
#include "stdio.h"
/******************************************************************************
         				#### WIRELESSCOM VARIABLES ####
******************************************************************************/
uint16_t Written_Bytes; /* is for save number of total converted buffer's characters*/
uint8_t cnt;

/******************************************************************************
         				#### WIRELESSCOM  FUNCTIONS ####
******************************************************************************/

/**
  * @brief Decimal, float and other formats are converted as character and save them into the TX buffer.
  * 		When TX buffer is fulfilled , it is sent by UART interface.
  * @note  Follow the transmitting rules, each if and else if has a why head for using
  * @param MissionUnit From_X, Packet type used for your specific purpose. Where do you  want to take it, select that.
  * 																	  @arg 0 : Sat_Carrier
  * 																 	  @arg 1 : Sat_Payload
  * 																  	  @arg 2 : Ground_Sation
  * @param MissionUnit To_Y, Packet type used for your specific purpose. Where do you want to send it, select that.
  * 																	  @arg 0 : Sat_Carrier
  * 																 	  @arg 1 : Sat_Payload
  * 																  	  @arg 2 : Ground_Sation
  * @param SubSys_WirelessCom_APP_HandleTypeDef *dev_WirelessComApp , created object for wireless communication
  * @retval NONE
  */
void SubSys_WirelessCom_Telemetry_Transfer_From_To(MissionUnit From_X, MissionUnit To_Y, SubSys_WirelessCom_APP_HandleTypeDef *dev_WirelessComApp){

	/*! Use it when working on Sat_Carrier flight software*/
	if(From_X == Sat_Carrier && To_Y == Sat_Payload){

		/*! Create message packet for Carrier for sending to the Payload*/
		SubSys_WirelessCom_Telemetry_Create_Packet_For(Sat_Carrier, dev_WirelessComApp);

				/* 8 pairs of '<>' and y Byte data are x Byte as total budget*/
//				Written_Bytes = sprintf(dev_WirelessComApp->Buffer.Temp,
//																		"<%.2f><%.2f><%.1f><%.1f><%.2f><%.4f><%.4f><%.1f>\n",
//																															dev_WirelessComApp->Variable.Carr_Pressure,
//																															dev_WirelessComApp->Variable.Carr_Temperature,
//																															dev_WirelessComApp->Variable.Carr_VertHeight,
//																															dev_WirelessComApp->Variable.Carr_VertSpeed,
//																															dev_WirelessComApp->Variable.Carr_BatteryVoltage,
//																															dev_WirelessComApp->Variable.Carr_GPS_Latitude,
//																															dev_WirelessComApp->Variable.Carr_GPS_Longitude,
//																															dev_WirelessComApp->Variable.Carr_GPS_Altitude);
				Written_Bytes = sprintf(dev_WirelessComApp->Buffer.Temp,
																		"C<%.2f><%.2f>",
																				   dev_WirelessComApp->Variable.Carr_Pressure,
																				   dev_WirelessComApp->Variable.Carr_VertHeight);

				for(cnt = 0 ; cnt < Written_Bytes ; cnt++){

					dev_WirelessComApp->Buffer.Tx[cnt+3] = dev_WirelessComApp->Buffer.Temp[cnt]; /*End of the array has \n character*/

				}

				for(uint8_t j=(cnt+3) ; j < SizeOf_Wireless_TX_Buff_Carrier ; j++){

					dev_WirelessComApp->Buffer.Tx[j] = '*';

				}

				//HAL_UART_Transmit(dev_WirelessComApp->huartX, dev_WirelessComApp->Buffer.Tx , (Written_Bytes+3), 1000);
				HAL_UART_Transmit(dev_WirelessComApp->huartX, dev_WirelessComApp->Buffer.Tx , SizeOf_Wireless_TX_Buff_Carrier, 1000);
	}

}


/**
 * @brief  : Creates 3 types packet for Carrier, Payload and Ground Station
 * @note   : Where do you want to send packet, select that, For example you are a courier and
 * 			 carry the packet to the selected home(MissionUnite x)
 *
 * @param  : MissionUnit x, Packet type used for your specific purpose @arg 0 : Sat_Carrier
 * 																	   @arg 1 : Sat_Payload
 * 																	   @arg 2 : Ground_Sation
 * @param  : SubSys_WirelessCom_APP_HandleTypeDef *dev_WirelessComApp , created object for wireless communication
 * @retval NONE
 */
void SubSys_WirelessCom_Telemetry_Create_Packet_For(MissionUnit x,SubSys_WirelessCom_APP_HandleTypeDef *dev_WirelessComApp){

	switch(x){
		case Sat_Carrier :

			/*-------------TARGET DEVICE ADDRESS AND CHANNEL INFO----------------*/
			/*! Target device will be Satellite's Payload*/
			dev_WirelessComApp->Buffer.Tx[0] = dev_WirelessComApp->Target_ADDH;
			dev_WirelessComApp->Buffer.Tx[1] = dev_WirelessComApp->Target_ADDL;
			dev_WirelessComApp->Buffer.Tx[2] = dev_WirelessComApp->Target_Ch;

			/*-------------YOUR DEVICE VARIABLE THAT YOU WİLL SEND----------------*/ /*Note : Will be system variable opposite to variables*/
			/*From MS5611*/
			dev_WirelessComApp->Variable.Carr_Pressure    = MS5611_Press;
			dev_WirelessComApp->Variable.Carr_Temperature = MS5611_Temp;
			dev_WirelessComApp->Variable.Carr_VertHeight  = MS5611_Altitude;
			dev_WirelessComApp->Variable.Carr_VertSpeed   = MS5611_VertSpeed;

			/*From ADC*/
			dev_WirelessComApp->Variable.Carr_BatteryVoltage = BatteryVoltage;

			/*From L-86GPS*/
			dev_WirelessComApp->Variable.Carr_GPS_Latitude  = GPS_Latitude;
			dev_WirelessComApp->Variable.Carr_GPS_Longitude = GPS_Longitude;
			dev_WirelessComApp->Variable.Carr_GPS_Altitude  = GPS_Altitude;

			/*! Each time a packet is generated, the count will increase by 1*/
			dev_WirelessComApp->Variable.NumOfPacket++;

			break;

		   }

}


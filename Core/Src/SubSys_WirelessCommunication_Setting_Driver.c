
/******************************************************************************
         				#### WIRELESSCOM INCLUDES ####
******************************************************************************/
#include "SubSys_WirelessCommunication_Setting_Driver.h"
#include "string.h"
/******************************************************************************
         				#### WIRELESSCOM VARIABLES ####
******************************************************************************/
/*! Is filled by lora's parameter variables at SubSys_WirelessCom_Config_Init function*/
uint8_t ParamsLoraToBeSend[11];
uint8_t ParamsLoraToBeGet[11];



/******************************************************************************
         				#### WIRELESSCOM  FUNCTIONS ####
******************************************************************************/
void SubSys_WirelessCom_Config_Init(SubSys_WirelesscomConfig_HandleTypeDef    *dev){

	/*----------------Implement selected mode -------------------*/
	SubSys_WirelessCom_Config_WORK_MODE(dev);

	/*----------------Select your special parameters-------------------*/
	dev->param.SerialPortRate 		= UART_Rate_is_9600;
	dev->param.ParityBit 			= p8N1;
	dev->param.AirDataRate    		= Air_Data_Rate_2d_4k;

	dev->param.SubPacket 			= bytes_200;
	dev->param.AmbientNoise_SW 		= EnableFea;
	dev->param.TX_Power 			= dBm30;

	dev->param.dev_Channel 			= 0x10;

	dev->param.RSSIByte_SW 			= EnableFea ;
	dev->param.TransmissionMethod 	= Fixed_t ;
	dev->param.LBT_SW 				= DisableFea;
	dev->param.WorCycle 			= ms500;


	/*----------------Save parameters into the "dev" object-------------------*/
	dev->ADDH = 0x19;
	dev->ADDL = 0x23;

	dev->REG0 = ((dev->param.SerialPortRate << 5) | (dev->param.ParityBit  << 3) | (dev->param.AirDataRate  << 0));
	dev->REG1 = ((dev->param.SubPacket << 6) | (dev->param.AmbientNoise_SW  << 5) | (dev->param.TX_Power  << 0));
	dev->REG2 = (dev->param.dev_Channel << 0);
	dev->REG3 = ((dev->param.RSSIByte_SW << 7) | (dev->param.TransmissionMethod << 6) | (dev->param.LBT_SW << 4) | (dev->param.WorCycle << 0));
	dev->REG_CRYPT_H = 0;
	dev->REG_CRYPT_L = 0;

	/*! Sets the selected parameter and sends it to the wireless communication device */
	SubSys_WirelessCom_Config_SET_REG(dev);

	/*! Read configuration register's value and checks with the correct value */
	 SubSys_WirelessCom_Config_READ_REG(dev);

}


void SubSys_WirelessCom_Config_SET_REG(SubSys_WirelesscomConfig_HandleTypeDef   *dev){

	uint16_t cnt = 0;

		/*! Save register variables into the ParamsLoraToBeSend array for sending lora chip at once*/
		ParamsLoraToBeSend[cnt] = writeCmnd; 					cnt++;		/* Command name */
		ParamsLoraToBeSend[cnt] = REG_BaseAddress; 				cnt++;		/* Starting addres */
		ParamsLoraToBeSend[cnt] = 0x08;							cnt++; 		/* Size of written bytes */
		ParamsLoraToBeSend[cnt] = dev->ADDH; 					cnt++; 		/* Addres High byte*/
		ParamsLoraToBeSend[cnt] = dev->ADDL; 					cnt++;		/* Addres Low byte */
		ParamsLoraToBeSend[cnt] = dev->REG0; 					cnt++;		/* Parameter register values*/
		ParamsLoraToBeSend[cnt] = dev->REG1; 					cnt++;
		ParamsLoraToBeSend[cnt] = dev->REG2; 					cnt++;
		ParamsLoraToBeSend[cnt] = dev->REG3; 					cnt++;
		ParamsLoraToBeSend[cnt] = dev->REG_CRYPT_H; 			cnt++;
		ParamsLoraToBeSend[cnt] = dev->REG_CRYPT_L; 			cnt++;

	/*! Write all the array's values into the LoRa's registers */
	HAL_UART_Transmit(dev->interface.huart, ParamsLoraToBeSend, sizeof(ParamsLoraToBeSend), 1000);

}


void SubSys_WirelessCom_Config_READ_REG(SubSys_WirelesscomConfig_HandleTypeDef    *dev){

	uint16_t cnt = 0;

	uint8_t ParamLoraforRead[3]={0};

	ParamLoraforRead[cnt] = readCmnd; 					cnt++;				/* Command name */
	ParamLoraforRead[cnt] = REG_BaseAddress; 			cnt++;				/* Starting addres */
	ParamLoraforRead[cnt] = sizeof(ParamsLoraToBeGet); 	cnt++; 				/* Size of written bytes */
	HAL_Delay(20);
	/*! Send read command to get values from LoRa's chip */
	HAL_UART_Transmit(dev->interface.huart, ParamLoraforRead, 3, 1000);

	HAL_UARTEx_ReceiveToIdle_DMA(dev->interface.huart, ParamsLoraToBeGet, sizeof(ParamsLoraToBeGet));
	__HAL_DMA_DISABLE_IT(dev->interface.hdma_usart_rx, DMA_IT_HT);


	for(int i = 3 ; i < sizeof(ParamsLoraToBeGet) ; i++)
	{

		if(ParamsLoraToBeSend[i] != ParamsLoraToBeGet[i])
		{
			/*Stop the algorithm until solving the problem*/
			while(1);
		}
		else
		{
			continue;
		}

	}

}


void SubSys_WirelessCom_Config_WORK_MODE(SubSys_WirelesscomConfig_HandleTypeDef    *dev){

	switch(dev->Mode_SW){

		case NormalMode :	/*! UART and wireless channel are open, transparent transmission is on*/
			HAL_GPIO_WritePin(dev->interface.GPIOx, dev->LORA_PIN_M1, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(dev->interface.GPIOx, dev->LORA_PIN_M0, GPIO_PIN_RESET);
		break;

		case WORsending :	/*! WOR Transmitter (it sends packet in every period)*/
			HAL_GPIO_WritePin(dev->interface.GPIOx, dev->LORA_PIN_M1, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(dev->interface.GPIOx, dev->LORA_PIN_M0, GPIO_PIN_SET);
		break;

		case WORreceiving :	/*! WOR Receiver (it sends packet in every period)*/
			HAL_GPIO_WritePin(dev->interface.GPIOx, dev->LORA_PIN_M1, GPIO_PIN_SET);
			HAL_GPIO_WritePin(dev->interface.GPIOx, dev->LORA_PIN_M0, GPIO_PIN_RESET);
		break;

		case DeepSleep :	/*! Module goes to sleep so provides you to configure settings*/
			HAL_GPIO_WritePin(dev->interface.GPIOx, dev->LORA_PIN_M1, GPIO_PIN_SET);
			HAL_GPIO_WritePin(dev->interface.GPIOx, dev->LORA_PIN_M0, GPIO_PIN_SET);
		break;

	}

}

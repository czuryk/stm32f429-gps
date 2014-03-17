#include "usartlib.h"
#include <stdio.h>
#include <string.h>
#include "main.h"


//**************************************************************************************************
// Declarations and definitions
//**************************************************************************************************

// Variables for GPIO and USART configuration
GPIO_InitTypeDef USART_CfgGPIO;
USART_InitTypeDef USART_CfgUSART;

#define MAX_STRLEN 80 // this is the maximum string length of our string in characters

char received_string[MAX_STRLEN+1]; // this will hold the recieved string
static uint8_t cnt = 0; // this counter is used to determine the string length


// USART_Init()

void InitUSART()
{
	__enable_irq();
	#if (USART_MODULE_IN_USE == USART_MODULE1)

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_StructInit(&USART_CfgGPIO);
	USART_CfgGPIO.GPIO_Mode = GPIO_Mode_AF;
	USART_CfgGPIO.GPIO_Pin = GPIO_Pin_9;
	USART_CfgGPIO.GPIO_Speed = GPIO_Speed_50MHz;
	USART_CfgGPIO.GPIO_OType = GPIO_OType_PP;
	USART_CfgGPIO.GPIO_PuPd = GPIO_PuPd_UP;
 	GPIO_Init(GPIOA, &USART_CfgGPIO);
	
	USART_CfgGPIO.GPIO_Mode = GPIO_Mode_AF;
	USART_CfgGPIO.GPIO_Pin = GPIO_Pin_10;
	USART_CfgGPIO.GPIO_Speed = GPIO_Speed_50MHz;
	USART_CfgGPIO.GPIO_OType = GPIO_OType_PP;
	USART_CfgGPIO.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &USART_CfgGPIO);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	
	USART_StructInit(&USART_CfgUSART);
	USART_CfgUSART.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_CfgUSART.USART_BaudRate = USART_BAUDRATE;
	USART_Init(USART1, &USART_CfgUSART);	
	
	NVIC_EnableIRQ(USART1_IRQn);
	USART_Cmd(USART1, ENABLE);
	
	#endif // #if (USART_MODULE_IN_USE == USART_MODULE1)
	
	#if (USART_MODULE_IN_USE == USART_MODULE2)

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_StructInit(&USART_CfgGPIO);
	USART_CfgGPIO.GPIO_Mode = GPIO_Mode_AF;
	USART_CfgGPIO.GPIO_Pin = GPIO_Pin_2;
	USART_CfgGPIO.GPIO_Speed = GPIO_Speed_50MHz;
	USART_CfgGPIO.GPIO_OType = GPIO_OType_PP;
	USART_CfgGPIO.GPIO_PuPd = GPIO_PuPd_UP;
 	GPIO_Init(GPIOA, &USART_CfgGPIO);
	
	USART_CfgGPIO.GPIO_Mode = GPIO_Mode_AF;
	USART_CfgGPIO.GPIO_Pin = GPIO_Pin_3;
	USART_CfgGPIO.GPIO_Speed = GPIO_Speed_50MHz;
	USART_CfgGPIO.GPIO_OType = GPIO_OType_PP;
	USART_CfgGPIO.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &USART_CfgGPIO);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
	
	USART_StructInit(&USART_CfgUSART);
	USART_CfgUSART.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_CfgUSART.USART_BaudRate = USART_BAUDRATE;
	USART_Init(USART2, &USART_CfgUSART);	
	
	NVIC_EnableIRQ(USART2_IRQn);
	USART_Cmd(USART2, ENABLE);
	
	#endif // #if (USART_MODULE_IN_USE == USART_MODULE2)
	
	#if (USART_MODULE_IN_USE == USART_MODULE3)

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_StructInit(&USART_CfgGPIO);
	USART_CfgGPIO.GPIO_Mode = GPIO_Mode_AF;
	USART_CfgGPIO.GPIO_Pin = GPIO_Pin_10;
	USART_CfgGPIO.GPIO_Speed = GPIO_Speed_50MHz;
	USART_CfgGPIO.GPIO_OType = GPIO_OType_PP;
	USART_CfgGPIO.GPIO_PuPd = GPIO_PuPd_UP;
 	GPIO_Init(GPIOB, &USART_CfgGPIO);
	
	USART_CfgGPIO.GPIO_Mode = GPIO_Mode_AF;
	USART_CfgGPIO.GPIO_Pin = GPIO_Pin_11;
	USART_CfgGPIO.GPIO_Speed = GPIO_Speed_50MHz;
	USART_CfgGPIO.GPIO_OType = GPIO_OType_PP;
	USART_CfgGPIO.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOB, &USART_CfgGPIO);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
	
	USART_StructInit(&USART_CfgUSART);
	USART_CfgUSART.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_CfgUSART.USART_BaudRate = USART_BAUDRATE;
	USART_Init(USART3, &USART_CfgUSART);	
	
	NVIC_EnableIRQ(USART3_IRQn);
	USART_Cmd(USART3, ENABLE);
	
	#endif // #if (USART_MODULE_IN_USE == USART_MODULE3)
	
	#if (USART_MODULE_IN_USE == USART_MODULE6)

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_StructInit(&USART_CfgGPIO);
	USART_CfgGPIO.GPIO_Mode = GPIO_Mode_AF;
	USART_CfgGPIO.GPIO_Pin = GPIO_Pin_6;
	USART_CfgGPIO.GPIO_Speed = GPIO_Speed_50MHz;
	USART_CfgGPIO.GPIO_OType = GPIO_OType_PP;
	USART_CfgGPIO.GPIO_PuPd = GPIO_PuPd_UP;
 	GPIO_Init(GPIOC, &USART_CfgGPIO);
	
	USART_CfgGPIO.GPIO_Mode = GPIO_Mode_AF;
	USART_CfgGPIO.GPIO_Pin = GPIO_Pin_7;
	USART_CfgGPIO.GPIO_Speed = GPIO_Speed_50MHz;
	USART_CfgGPIO.GPIO_OType = GPIO_OType_PP;
	USART_CfgGPIO.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOC, &USART_CfgGPIO);
	
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);
	
	USART_StructInit(&USART_CfgUSART);
	USART_CfgUSART.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_CfgUSART.USART_BaudRate = USART_BAUDRATE;
	USART_Init(USART6, &USART_CfgUSART);	
	
	NVIC_EnableIRQ(USART6_IRQn);
	USART_Cmd(USART6, ENABLE);
	
	#endif // #if (USART_MODULE_IN_USE == USART_MODULE6)
	
} // End of function USART_Init()



// USART_SendData()

void USART_Send(char *SendData) {
	int i = 0;

	#if (USART_MODULE_IN_USE == USART_MODULE1)
	while (SendData[i] != 0) {
		while (!USART_GetFlagStatus(USART1, USART_FLAG_TC));
		USART_SendData(USART1, SendData[i++]);
	}	
	#endif // #if (USART_MODULE_IN_USE == USART_MODULE1)
	
	#if (USART_MODULE_IN_USE == USART_MODULE2)
	while (SendData[i] != 0) {
		while (!USART_GetFlagStatus(USART2, USART_FLAG_TC));
		USART_SendData(USART2, SendData[i++]);
	}
	#endif // #if (USART_MODULE_IN_USE == USART_MODULE2)
	
	#if (USART_MODULE_IN_USE == USART_MODULE3)
	while (SendData[i] != 0) {
		while (!USART_GetFlagStatus(USART3, USART_FLAG_TC));
		USART_SendData(USART3, SendData[i++]);
	}	
	#endif // #if (USART_MODULE_IN_USE == USART_MODULE3)
	
	#if (USART_MODULE_IN_USE == USART_MODULE6)
	while (SendData[i] != 0) {
		while (!USART_GetFlagStatus(USART6, USART_FLAG_TC));
		USART_SendData(USART6, SendData[i++]);
	}	
	#endif // #if (USART_MODULE_IN_USE == USART_MODULE6)
} // end of USART_SendData()



void USART_StartReceive(void) {
	#if (USART_MODULE_IN_USE == USART_MODULE1)
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	#endif // #if (USART_MODULE_IN_USE == USART_MODULE1)
	
	#if (USART_MODULE_IN_USE == USART_MODULE2)
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	#endif // #if (USART_MODULE_IN_USE == USART_MODULE2)
	
	#if (USART_MODULE_IN_USE == USART_MODULE3)
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	#endif // #if (USART_MODULE_IN_USE == USART_MODULE3)
	
	#if (USART_MODULE_IN_USE == USART_MODULE6)
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);
	#endif // #if (USART_MODULE_IN_USE == USART_MODULE6)
} // end



void USART_StopReceive(void) {
	#if (USART_MODULE_IN_USE == USART_MODULE1)
	USART_ITConfig(USART1, USART_IT_TC, DISABLE);	
	#endif // #if (USART_MODULE_IN_USE == USART_MODULE1)
	
	#if (USART_MODULE_IN_USE == USART_MODULE2)
	USART_ITConfig(USART2, USART_IT_TC, DISABLE);
	#endif // #if (USART_MODULE_IN_USE == USART_MODULE2)
	
	#if (USART_MODULE_IN_USE == USART_MODULE3)
	USART_ITConfig(USART3, USART_IT_TC, DISABLE);
	#endif // #if (USART_MODULE_IN_USE == USART_MODULE3)
	
	#if (USART_MODULE_IN_USE == USART_MODULE6)
	USART_ITConfig(USART6, USART_IT_TC, DISABLE);
	#endif // #if (USART_MODULE_IN_USE == USART_MODULE6)
} // end



// Interrupts
// USARTx_IRQHandler()

#if (USART_MODULE_IN_USE == USART_MODULE1)
void USART1_IRQHandler() {
int n = 0;
	// Recieve data
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
		char t = USART_ReceiveData(USART1); // the character from the USART1 data register is saved in t
		if( (t != '\\') && (t != 10) && (t != 13) && (cnt < MAX_STRLEN) ) {
			received_string[cnt] = t;
			cnt++;
		}
		else { // otherwise reset the character counter and print the received string
			cnt = 0;
			if ((unsigned)strlen(received_string) > 0) {
				GetUSARTData(received_string); // Callback function to main code
				for (n = 0; n < MAX_STRLEN+1; n++) {received_string[n] = 0;} // Cleanup
			}
		}		
	}
} // End of USART1_IRQHandler() 
#endif // #if (USART_MODULE_IN_USE == USART_MODULE1)



#if (USART_MODULE_IN_USE == USART_MODULE2)
void USART2_IRQHandler() {
int n = 0;
	// Recieve data
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
		char t = USART_ReceiveData(USART2); // the character from the USART1 data register is saved in t
		if( (t != '\\') && (t != 10) && (t != 13) && (cnt < MAX_STRLEN) ) {
			received_string[cnt] = t;
			cnt++;
		}
		else { // otherwise reset the character counter and print the received string
			cnt = 0;
			if ((unsigned)strlen(received_string) > 0) {
				GetUSARTData(received_string); // Callback function to main code
				for (n = 0; n < MAX_STRLEN+1; n++) {received_string[n] = 0;} // Cleanup
			}
		}		
	}
} // End of USART2_IRQHandler() 
#endif // #if (USART_MODULE_IN_USE == USART_MODULE2)



#if (USART_MODULE_IN_USE == USART_MODULE3)
void USART3_IRQHandler() {
int n = 0;
	// Recieve data
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
		char t = USART_ReceiveData(USART3); // the character from the USART1 data register is saved in t
		if( (t != '\\') && (t != 10) && (t != 13) && (cnt < MAX_STRLEN) ) {
			received_string[cnt] = t;
			cnt++;
		}
		else { // otherwise reset the character counter and print the received string
			cnt = 0;
			if ((unsigned)strlen(received_string) > 0) {
				GetUSARTData(received_string); // Callback function to main code
				for (n = 0; n < MAX_STRLEN+1; n++) {received_string[n] = 0;} // Cleanup
			}
		}		
	}
} // End of USART3_IRQHandler() 
#endif // #if (USART_MODULE_IN_USE == USART_MODULE3)



#if (USART_MODULE_IN_USE == USART_MODULE6)
void USART6_IRQHandler() {
int n = 0;
	// Recieve data
	if (USART_GetITStatus(USART6, USART_IT_RXNE) != RESET) {
		char t = USART_ReceiveData(USART6); // the character from the USART1 data register is saved in t
		if( (t != '\\') && (t != 10) && (t != 13) && (cnt < MAX_STRLEN) ) {
			received_string[cnt] = t;
			cnt++;
		}
		else { // otherwise reset the character counter and print the received string
			cnt = 0;
			if ((unsigned)strlen(received_string) > 0) {
				GetUSARTData(received_string); // Callback function to main code
				for (n = 0; n < MAX_STRLEN+1; n++) {received_string[n] = 0;} // Cleanup
			}
		}		
	}
} // End of USART6_IRQHandler() 
#endif // #if (USART_MODULE_IN_USE == USART_MODULE6)



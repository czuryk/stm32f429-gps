
#ifndef STM32F4xx_USART_H
#define STM32F4xx_USART_H



//**************************************************************************************************
// Includes
//**************************************************************************************************

#include "stm32f4xx.h"

// Get RCC functions from Standard Peripheral Library
#include "stm32f4xx_rcc.h"

// Get GPIO functions from Standard Peripheral Library
#include "stm32f4xx_gpio.h"

// Get USART functions from Standard Peripheral Library
#include "stm32f4xx_usart.h"




//**************************************************************************************************
// Declarations and definitions
//**************************************************************************************************

// Define TRUE, FALSE and bool if not defined
typedef enum {FALSE = 0, TRUE = !FALSE} bool;

// USART module numbers
#define USART_MODULE1												(1)
#define USART_MODULE2												(2)
#define USART_MODULE3												(3)
#define USART_MODULE6												(6)

// Library configuration
// Select USART number
#define USART_MODULE_IN_USE							(USART_MODULE1)

//Set USART baudrate
#define USART_BAUDRATE										(9600)

// USART functions prototypes
extern void InitUSART(void);
extern void USART_Send(char *SendData);
extern void USART_StartReceive(void);
extern void USART_StopReceive(void);

#if (USART_MODULE_NUMBER == USART_MODULE1)
extern void USART1_IRQHandler(void);
#endif // #if (USART_MODULE_NUMBER == USART_MODULE1)

#if (USART_MODULE_NUMBER == USART_MODULE2)
extern void USART2_IRQHandler(void);
#endif // #if (USART_MODULE_NUMBER == USART_MODULE2)

#if (USART_MODULE_NUMBER == USART_MODULE3)
extern void USART3_IRQHandler(void);
#endif // #if (USART_MODULE_NUMBER == USART_MODULE3)

#if (USART_MODULE_NUMBER == USART_MODULE6)
extern void USART6_IRQHandler(void);
#endif // #if (USART_MODULE_NUMBER == USART_MODULE6)



#endif // #ifndef STM32F4xx_USART_H

//****************************************** end of file *******************************************

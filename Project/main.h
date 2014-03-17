/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>

#include "usartlib.h"
#include "gps.h"
#include "swo.h"
#include "stm32f4xx.h"

#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"

/* Private define ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/ 
/* Exported functions ------------------------------------------------------- */
extern void GetUSARTData(const char* str);
extern void SatAdvData(void);


#endif /* __MAIN_H */

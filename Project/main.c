/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "logo.h"
#include <cstdlib>


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

int WAIT_SAT_DRAW = 0;

LTDC_ColorKeying_InitTypeDef   LTDC_colorkeying_InitStruct;
/* Private function prototypes -----------------------------------------------*/
static void LCD_Config(void);
static void LCD_AF_GPIOConfig(void);
static void delay(__IO uint32_t nCount);


int compare( const void* a, const void* b)
{
   int int_a = * ( (int*) a );
   int int_b = * ( (int*) b );

   // an easy expression for comparing
   return (int_a > int_b) - (int_a < int_b);
}



/* ------ Callback functions ---------- */
//void SatAdvData(void) {
int UpdateSats(void) {
	int i, n;
	int sat_count;
	char sat_num[2];
	char sat_nums[30];
	int line_from_x, line_from_y;
	int line_count;
	int text_line_y;

	//USART_StopReceive();

	if (data_ready == 0) {return 0;}
	WAIT_SAT_DRAW = 1;
/*
	for (i = 0; i < 50; i++) {
		if (satsarr[i] > -1) {printf("sat: %d, signal: %d\n", i, satsarr[i]);}
	}
*/
	
	for (n = 0; n < 30+1; n++) {sat_nums[n] = 0;} // Cleanup
	i = 0;
	n = 0;
	sat_count = 0;
	line_count = 0;
	line_from_x = 7;
	line_from_y	= 180;
	text_line_y = 23;

	LCD_SetTextColor(LCD_COLOR_GREEN);
	LCD_SetFont(&Font8x8);
	LCD_DisplayStringLine(LINE(23), (uint8_t*)"                             ");
	LCD_DisplayStringLine(LINE(38), (uint8_t*)"                             ");
	for (i = 0; i < 10; i++) {
		LCD_SetTextColor(LCD_COLOR_BLACK);
		LCD_DrawLine(line_from_x+(i * 24), 183-80, 80, LCD_DIR_VERTICAL);
		LCD_DrawLine(line_from_x+(i * 24)+1, 183-80, 80, LCD_DIR_VERTICAL);
		LCD_DrawLine(line_from_x+(i * 24), 298-80, 80, LCD_DIR_VERTICAL);
		LCD_DrawLine(line_from_x+(i * 24)+1, 298-80, 80, LCD_DIR_VERTICAL);
	}

	for (i = 0; i < 50; i++) {
		if (satsarr[i] > -1) {
			if (i < 10) {sprintf(sat_num, "0%d ", i);} else {sprintf(sat_num, "%d ", i);}
			if (sat_count == 20) {break;}
			if (sat_count == 10) {
				LCD_SetTextColor(LCD_COLOR_GREEN);
				LCD_SetFont(&Font8x8);				
				LCD_DisplayStringLine(LINE(text_line_y), (uint8_t*)sat_nums);
				for (n = 0; n < 30+1; n++) {sat_nums[n] = 0;} // Cleanup
				line_count = 0;
				line_from_y	= 295;
				text_line_y = 38;
			}
			
			if (sat_count > 0) {strcat(strcpy(sat_nums, sat_nums), sat_num);} else {strcpy(sat_nums, sat_num);}

			LCD_SetTextColor(LCD_COLOR_BLACK);
			LCD_DrawLine(line_from_x+(line_count * 24), line_from_y-80, 80, LCD_DIR_VERTICAL);
			LCD_DrawLine(line_from_x+(line_count * 24)+1, line_from_y-80, 80, LCD_DIR_VERTICAL);
			
			if (satsarr[i] < 15) {LCD_SetTextColor(LCD_COLOR_RED);} else {LCD_SetTextColor(LCD_COLOR_GREEN);}			
			LCD_DrawLine(line_from_x+(line_count * 24), line_from_y-satsarr[i]+1, satsarr[i]+1, LCD_DIR_VERTICAL);
			LCD_DrawLine(line_from_x+(line_count * 24)+1, line_from_y-satsarr[i]+1, satsarr[i]+1, LCD_DIR_VERTICAL);
			sat_count++;
			line_count++;
		}
	}
	LCD_SetTextColor(LCD_COLOR_GREEN);
	LCD_SetFont(&Font8x8);
	LCD_DisplayStringLine(LINE(text_line_y), (uint8_t*)sat_nums);	
	//USART_StartReceive();	
	WAIT_SAT_DRAW = 0;
	return 0;
}


void GetUSARTData(const char* str) {
	char time[7];
	char date[7];
	char lat[10];
	char lon[10];
	char text[20];
	int sats;

	if (strstr(str, "GPRMC") != NULL) {
		GPS_GetCoord(str, lat, lon);
		GPS_GetDateTime(str, date, time);


		printf("Lat: %s\n", lat);
		printf("Lon: %s\n", lon);	
		printf("Date: %s\n", date);
		printf("Time: %s\n", time);

		LCD_SetTextColor(LCD_COLOR_CYAN);		
		LCD_SetFont(&Font16x24);		
		sprintf(text, "LAT: %s", lat);
		LCD_DisplayStringLine(LINE(0), (uint8_t*)text);
		sprintf(text, "LON: %s", lon);
		LCD_DisplayStringLine(LINE(1), (uint8_t*)text);	

		LCD_SetTextColor(LCD_COLOR_YELLOW);
		LCD_SetFont(&Font12x12);
		sprintf(text, "TIME: %s", time);
		LCD_DisplayStringLine(LINE(5), (uint8_t*)text);
		sprintf(text, "DATE: %s", date);
		LCD_DisplayStringLine(LINE(6), (uint8_t*)text);
		
		
		
	}
	if (strstr(str, "GPGSV") != NULL) {
		if (WAIT_SAT_DRAW == 0) {GPS_SatInfo(str, sats);}
	}
	printf("%s\n", str);
}


/* Private functions ---------------------------------------------------------*/


int main(void) {	
	int i = 0, n = 0;
	char text[5];
	for (n = 0; n < 5+1; n++) {text[n] = 0;} // Cleanup
	
	// connected
	// PA10 - RX, PA9 - TX

  /* User button will be used */
  STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_GPIO);
  

  // LCD Configuration
  LCD_Config();
  
  // Enable Layer1
  LTDC_LayerCmd(LTDC_Layer1, ENABLE);
  
  // Reload configuration of Layer1
  LTDC_ReloadConfig(LTDC_IMReload);
  
  // Enable The LCD
  LTDC_Cmd(ENABLE);	

	LTDC_LayerPosition(LTDC_Layer1, 90, 100);
	LTDC_ReloadConfig(LTDC_IMReload);

	delay(100000);
	for (n = 255; n > 0; n--) {
		LCD_SetTransparency(n);
		LTDC_ReloadConfig(LTDC_IMReload);
		delay(300);
	}




  /* LCD initialization */
  LCD_Init();
  
  /* LCD Layer initialization */
	LCD_LayerInit();
	
  // Enable Layer1
  LTDC_LayerCmd(LTDC_Layer1, ENABLE);	

  /* Enable the LTDC */
  LTDC_Cmd(ENABLE);

	/* Set LCD foreground layer */
  LCD_SetLayer(LCD_FOREGROUND_LAYER);

		LCD_Clear(LCD_COLOR_BLACK);	
		LCD_SetBackColor(LCD_COLOR_BLACK);
		LCD_SetTextColor(LCD_COLOR_WHITE);

    InitUSART();
		USART_StartReceive();


while(1) {
		UpdateSats();
//		sprintf(text, "%i", i);
//		LCD_SetTextColor(LCD_COLOR_CYAN);		
//		LCD_SetFont(&Font16x24);
//		LCD_DisplayStringLine(LINE(4), (uint8_t*)text);
		delay(100000);
		i++;
		}
}






/**
  * @brief LCD configuration.
  * @note  This function Configure tha LTDC peripheral :
  *        1) Configure the Pixel Clock for the LCD
  *        2) Configure the LTDC Timing and Polarity
  *        3) Configure the LTDC Layer 1 :
  *           - The frame buffer is located at SDRAM buffer
  *           - The Layer size configuration : 240x320
  *        4) Configure the Color Keying for Layer 1.    
  * @retval
  *  None
  */

static void LCD_Config(void)
{
  LTDC_InitTypeDef               LTDC_InitStruct;
  LTDC_Layer_InitTypeDef         LTDC_Layer_InitStruct;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  
  /* Configure NCS in Output Push-Pull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  /* Configure the LCD Control pins ------------------------------------------*/
  LCD_CtrlLinesConfig();
  LCD_ChipSelect(DISABLE);
  LCD_ChipSelect(ENABLE);

  /* Configure the LCD_SPI interface -----------------------------------------*/
  LCD_SPIConfig(); 
 
  /* Power on the LCD --------------------------------------------------------*/
  LCD_PowerOn();
    
  /* Enable the LTDC Clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_LTDC, ENABLE);
  
  /* Enable the DMA2D Clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2D, ENABLE); 
  
  /* Configure the LCD Control pins */
  LCD_AF_GPIOConfig();  
  
  /* Configure the FMC Parallel interface : SDRAM is used as Frame Buffer for LCD */
  SDRAM_Init();
  
/* Enable Pixel Clock --------------------------------------------------------*/

  /* Configure PLLSAI prescalers for LCD */
  /* PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz */
  /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAI_N = 192 Mhz */
  /* PLLLCDCLK = PLLSAI_VCO Output/PLLSAI_R = 192/3 = 64 Mhz */
  /* LTDC clock frequency = PLLLCDCLK / RCC_PLLSAIDivR = 64/8 = 8 Mhz */
  RCC_PLLSAIConfig(192, 7, 3);
  RCC_LTDCCLKDivConfig(RCC_PLLSAIDivR_Div8);
  
  /* Enable PLLSAI Clock */
  RCC_PLLSAICmd(ENABLE);
  /* Wait for PLLSAI activation */
  while(RCC_GetFlagStatus(RCC_FLAG_PLLSAIRDY) == RESET)
  {
  }

/* LTDC Initialization -------------------------------------------------------*/

  /* Initialize the horizontal synchronization polarity as active low*/
  LTDC_InitStruct.LTDC_HSPolarity = LTDC_HSPolarity_AL;     
  /* Initialize the vertical synchronization polarity as active low */  
  LTDC_InitStruct.LTDC_VSPolarity = LTDC_VSPolarity_AL;     
  /* Initialize the data enable polarity as active low */ 
  LTDC_InitStruct.LTDC_DEPolarity = LTDC_DEPolarity_AL;     
  /* Initialize the pixel clock polarity as input pixel clock */ 
  LTDC_InitStruct.LTDC_PCPolarity = LTDC_PCPolarity_IPC;
  
  /* Timing configuration */
    /* Configure horizontal synchronization width */     
  LTDC_InitStruct.LTDC_HorizontalSync = 9;
  /* Configure vertical synchronization height */
  LTDC_InitStruct.LTDC_VerticalSync = 1;
  /* Configure accumulated horizontal back porch */
  LTDC_InitStruct.LTDC_AccumulatedHBP = 29; 
  /* Configure accumulated vertical back porch */
  LTDC_InitStruct.LTDC_AccumulatedVBP = 3;  
  /* Configure accumulated active width */  
  LTDC_InitStruct.LTDC_AccumulatedActiveW = 269;
  /* Configure accumulated active height */
  LTDC_InitStruct.LTDC_AccumulatedActiveH = 323;
  /* Configure total width */
  LTDC_InitStruct.LTDC_TotalWidth = 279; 
  /* Configure total height */
  LTDC_InitStruct.LTDC_TotalHeigh = 327;
  
  LTDC_Init(&LTDC_InitStruct);
  
  /* Configure R,G,B component values for LCD background color */                   
  LTDC_InitStruct.LTDC_BackgroundRedValue = 0;            
  LTDC_InitStruct.LTDC_BackgroundGreenValue = 0;          
  LTDC_InitStruct.LTDC_BackgroundBlueValue = 0; 
            
  LTDC_Init(&LTDC_InitStruct);
  
/* LTDC initialization end ---------------------------------------------------*/

/* Layer1 Configuration ------------------------------------------------------*/  
    
  /* Windowing configuration */ 
  /* In this case all the active display area is used to display a picture then :
     Horizontal start = horizontal synchronization + Horizontal back porch = 30 
     Horizontal stop = Horizontal start + window width -1 = 30 + 240 -1
     Vertical start   = vertical synchronization + vertical back porch     = 4
     Vertical stop   = Vertical start + window height -1  = 4 + 320 -1      */ 
  LTDC_Layer_InitStruct.LTDC_HorizontalStart = 30;
  LTDC_Layer_InitStruct.LTDC_HorizontalStop = (240 + 30 - 1); 
  LTDC_Layer_InitStruct.LTDC_VerticalStart = 4;
  LTDC_Layer_InitStruct.LTDC_VerticalStop = (160 + 4 - 1); //320
  
  /* Pixel Format configuration*/           
  LTDC_Layer_InitStruct.LTDC_PixelFormat = LTDC_Pixelformat_RGB565;
  
  /* Alpha constant (255 totally opaque) */
  LTDC_Layer_InitStruct.LTDC_ConstantAlpha = 255; 
  
  /* Configure blending factors */       
  LTDC_Layer_InitStruct.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_CA;    
  LTDC_Layer_InitStruct.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_CA;  
  
  /* Default Color configuration (configure A,R,G,B component values) */          
  LTDC_Layer_InitStruct.LTDC_DefaultColorBlue = 255;        
  LTDC_Layer_InitStruct.LTDC_DefaultColorGreen = 255;       
  LTDC_Layer_InitStruct.LTDC_DefaultColorRed = 255;         
  LTDC_Layer_InitStruct.LTDC_DefaultColorAlpha = 255;   
   
  /* Input Address configuration */    
  LTDC_Layer_InitStruct.LTDC_CFBStartAdress = (uint32_t)&logo;
 
  /* the length of one line of pixels in bytes + 3 then :
     Line Lenth = Active high width x number of bytes per pixel + 3 
     Active high width         = 240 
     number of bytes per pixel = 2    (pixel_format : RGB565) 
  */
  LTDC_Layer_InitStruct.LTDC_CFBLineLength = ((60 * 2) + 3);
  
  /*  the pitch is the increment from the start of one line of pixels to the 
      start of the next line in bytes, then :
      Pitch = Active high width x number of bytes per pixel     
  */
  LTDC_Layer_InitStruct.LTDC_CFBPitch = (60 * 2);  
  
  /* configure the number of lines */
  LTDC_Layer_InitStruct.LTDC_CFBLineNumber = 60;
   
  LTDC_LayerInit(LTDC_Layer1, &LTDC_Layer_InitStruct);
  
  LTDC_DitherCmd(ENABLE);
}


static void LCD_AF_GPIOConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* Enable GPIOI, GPIOJ, GPIOG, GPIOF, GPIOH AHB Clocks */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | \
                         RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | \
                         RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG, ENABLE);

/* GPIOs Configuration */
/*
 +------------------------+-----------------------+----------------------------+
 +                       LCD pins assignment                                   +
 +------------------------+-----------------------+----------------------------+
 |  LCD_TFT R2 <-> PC.12  |  LCD_TFT G2 <-> PA.06 |  LCD_TFT B2 <-> PD.06      |
 |  LCD_TFT R3 <-> PB.00  |  LCD_TFT G3 <-> PG.10 |  LCD_TFT B3 <-> PG.11      |
 |  LCD_TFT R4 <-> PA.11  |  LCD_TFT G4 <-> PB.10 |  LCD_TFT B4 <-> PG.12      |
 |  LCD_TFT R5 <-> PA.12  |  LCD_TFT G5 <-> PB.11 |  LCD_TFT B5 <-> PA.03      |
 |  LCD_TFT R6 <-> PB.01  |  LCD_TFT G6 <-> PC.07 |  LCD_TFT B6 <-> PB.08      |
 |  LCD_TFT R7 <-> PG.06  |  LCD_TFT G7 <-> PD.03 |  LCD_TFT B7 <-> PB.09      |
 -------------------------------------------------------------------------------
          |  LCD_TFT HSYNC <-> PC.06  | LCDTFT VSYNC <->  PA.04 |
          |  LCD_TFT CLK   <-> PG.07  | LCD_TFT DE   <->  PF.10 |
           -----------------------------------------------------

*/

 /* GPIOA configuration */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_LTDC);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_6 | \
                             GPIO_Pin_11 | GPIO_Pin_12;
                             
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
  
 /* GPIOB configuration */  
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_LTDC);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | \
                             GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
  
  GPIO_Init(GPIOB, &GPIO_InitStruct);

 /* GPIOC configuration */
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_LTDC);
//  GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_LTDC);
  
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;// | GPIO_Pin_10;
                             
  GPIO_Init(GPIOC, &GPIO_InitStruct);

 /* GPIOD configuration */
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource3, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_LTDC);
  
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_6;
                             
  GPIO_Init(GPIOD, &GPIO_InitStruct);
  
 /* GPIOF configuration */
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource10, GPIO_AF_LTDC);
  
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
                             
  GPIO_Init(GPIOF, &GPIO_InitStruct);     

 /* GPIOG configuration */  
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource6, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource7, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource10, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource11, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource12, GPIO_AF_LTDC);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_10 | \
                             GPIO_Pin_11 | GPIO_Pin_12;
  
  GPIO_Init(GPIOG, &GPIO_InitStruct);
}

/**
  * @brief  Inserts a delay time.
  * @param  nCount: specifies the delay time length.
  * @retval None
  */
static void delay(__IO uint32_t nCount)
{
  __IO uint32_t index = 0; 
  //for(index = 100000*nCount; index != 0; index--)
	for(index = 100*nCount; index != 0; index--)	
  {
  }
}
#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

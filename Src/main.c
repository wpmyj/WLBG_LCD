/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include"HeadType.h"
#include "lcd.h"
#include "text.h"	
#include "GUI.h"

CRC_HandleTypeDef hcrc;
/* Private function prototypes -----------------------------------------------*/
void Stm32_Clock_Init(u32 plln,u32 pllm,u32 pllp,u32 pllq);
static void MX_GPIO_Init(void);
void MX_CRC_Init(void);
void GUIDEMO_Main(void);
extern void MainTask_test(void);
int main(void)
{
  HAL_Init();
  Stm32_Clock_Init(336,8,2,7);
	MX_CRC_Init();
	ROM_GT30L_Init();
//	LCD_Init();
	LED_GPIO_Config();
	GUI_Init();
	MX_GPIO_Init();
	TIM2_Config();
//	TIM3_Config();
	TIM3_PWM_Init(2000,42-1);    	//42M/42=1M的计数频率，自动重装载为500，那么PWM频率为1M/500=2kHZ
	USART1_Config();
	delay_ms(200);
	GUI_DispStringAt("MINI_STM32 STemWin test!",10,30);
	GUI_DispStringAt("MINI_STM32 STemWin test!",20,46);
	MainTask_test();
	GUIDEMO_Main();
  while (1)
  {
		LCD_Clear(BLACK);
	  Show_Str(20,20,16*2,"你好",BACK_COLOR,POINT_COLOR,16,0);
//		Show_Str(50,20,24*6,"我们是好孩子",BACK_COLOR,POINT_COLOR,24,0);
		Show_Str(40,50,6*32,"我们是好孩子",BACK_COLOR,POINT_COLOR,32,0);
		Show_Str(100,100,16*6,"Hello",BACK_COLOR,POINT_COLOR,32,0);
		Show_Str(10,130,12*6,"World",BACK_COLOR,POINT_COLOR,24,0);
		Show_Str(10 + 12*6,130,8*6,"World",BACK_COLOR,POINT_COLOR,16,0);
		Show_Str(32,162,64*4,"东ABC2区",BACK_COLOR,POINT_COLOR,64,0);
		delay_ms(1000);
		LCD_Clear(BLACK);
		POINT_COLOR = WHITE;
		Show_Str(40,0,80*4,"东12区",BACK_COLOR,POINT_COLOR,80,0);
		Show_Str(80,80,80*4," 住院 ",BACK_COLOR,POINT_COLOR,80,0);
		POINT_COLOR = RED;
		Show_Str(0,160,80*4,"(2096)袋",BACK_COLOR,POINT_COLOR,80,0);
		delay_ms(1000);
	}
}

/** System Clock Configuration
*/
void Stm32_Clock_Init(u32 plln,u32 pllm,u32 pllp,u32 pllq)
{
    HAL_StatusTypeDef ret = HAL_OK;
    RCC_OscInitTypeDef RCC_OscInitStructure; 
    RCC_ClkInitTypeDef RCC_ClkInitStructure;
    
    __HAL_RCC_PWR_CLK_ENABLE(); //使能PWR时钟
    
    //下面这个设置用来设置调压器输出电压级别，以便在器件未以最大频率工作
    //时使性能与功耗实现平衡。
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);//设置调压器输出电压级别1
    
    RCC_OscInitStructure.OscillatorType=RCC_OSCILLATORTYPE_HSE;    //时钟源为HSE
    RCC_OscInitStructure.HSEState=RCC_HSE_ON;                      //打开HSE
    RCC_OscInitStructure.PLL.PLLState=RCC_PLL_ON;//打开PLL
    RCC_OscInitStructure.PLL.PLLSource=RCC_PLLSOURCE_HSE;//PLL时钟源选择HSE
    RCC_OscInitStructure.PLL.PLLM=pllm; //主PLL和音频PLL分频系数(PLL之前的分频),取值范围:2~63.
    RCC_OscInitStructure.PLL.PLLN=plln; //主PLL倍频系数(PLL倍频),取值范围:64~432.  
    RCC_OscInitStructure.PLL.PLLP=pllp; //系统时钟的主PLL分频系数(PLL之后的分频),取值范围:2,4,6,8.(仅限这4个值!)
    RCC_OscInitStructure.PLL.PLLQ=pllq; //USB/SDIO/随机数产生器等的主PLL分频系数(PLL之后的分频),取值范围:2~15.
    ret=HAL_RCC_OscConfig(&RCC_OscInitStructure);//初始化
	
    if(ret!=HAL_OK) while(1);
    
    //选中PLL作为系统时钟源并且配置HCLK,PCLK1和PCLK2
    RCC_ClkInitStructure.ClockType=(RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStructure.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK;//设置系统时钟时钟源为PLL
    RCC_ClkInitStructure.AHBCLKDivider=RCC_SYSCLK_DIV1;//AHB分频系数为1
    RCC_ClkInitStructure.APB1CLKDivider=RCC_HCLK_DIV4; //APB1分频系数为4
    RCC_ClkInitStructure.APB2CLKDivider=RCC_HCLK_DIV2; //APB2分频系数为2
    ret=HAL_RCC_ClockConfig(&RCC_ClkInitStructure,FLASH_LATENCY_5);//同时设置FLASH延时周期为5WS，也就是6个CPU周期。
		
    if(ret!=HAL_OK) while(1);

	 //STM32F405x/407x/415x/417x Z版本的器件支持预取功能
	if (HAL_GetREVID() == 0x1001)
	{
		__HAL_FLASH_PREFETCH_BUFFER_ENABLE();  //使能flash预取
	}
}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pins : PC2 PC3 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);



}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif



void MX_CRC_Init(void)
{
  hcrc.Instance = CRC;
  HAL_CRC_Init(&hcrc);
}

void HAL_CRC_MspInit(CRC_HandleTypeDef* hcrc)
{

  if(hcrc->Instance==CRC)
  {
    __HAL_RCC_CRC_CLK_ENABLE();
  }
}

void HAL_CRC_MspDeInit(CRC_HandleTypeDef* hcrc)
{

  if(hcrc->Instance==CRC)
  {
    __HAL_RCC_CRC_CLK_DISABLE();
  }
} 


/******************* (C) COPYRIGHT 2015-2020 ????????? *****END OF FILE****/

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

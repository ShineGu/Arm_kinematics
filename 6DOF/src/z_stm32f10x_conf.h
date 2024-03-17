/****************************************************************************
	*	@笔者	：	W
	*	@日期	：	2019年12月03日
	*	@所属	：	杭州众灵科技
	*	@论坛	：	www.ZL-robot.com
 ****************************************************************************/

#ifndef __STM32F10x_CONF_H
#define __STM32F10x_CONF_H

/*******取消注释/注释下面的行，以启用/禁用外设头文件包含*******/
//#include "stm32f10x_adc.h"
//#include "stm32f10x_bkp.h"
//#include "stm32f10x_can.h"
//#include "stm32f10x_cec.h"
//#include "stm32f10x_crc.h"
//#include "stm32f10x_dac.h"
//#include "stm32f10x_dbgmcu.h"
//#include "stm32f10x_dma.h"
//#include "stm32f10x_exti.h"
//#include "stm32f10x_flash.h"
//#include "stm32f10x_fsmc.h"
#include "stm32f10x_gpio.h"
//#include "stm32f10x_i2c.h"
//#include "stm32f10x_iwdg.h"
//#include "stm32f10x_pwr.h"
#include "stm32f10x_rcc.h"
//#include "stm32f10x_rtc.h"
//#include "stm32f10x_sdio.h"
//#include "stm32f10x_spi.h"
#include "stm32f10x_tim.h"
//#include "stm32f10x_usart.h"
//#include "stm32f10x_wwdg.h"
#include "misc.h" 

/*******取消注释下面的行来扩展标准外设库驱动程序代码中的“assert_param”宏*******/
//#define USE_FULL_ASSERT    1

#ifdef  USE_FULL_ASSERT
	/***********************************************
		函数名称：	assert_param(expr) 
		功能介绍：	“assert_param”宏用于检查函数的参数。
		函数参数：	expr:	0 则调用assert_failed函数，该函数报告源文件的名称和失败调用的源行号;
											1 则不返回任何值。
		返回值：		无
	 ***********************************************/
  #define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
  void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert_param(expr) ((void)0)
#endif

#endif

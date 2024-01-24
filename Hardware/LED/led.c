/***
	*************************************************************************************************
	*	@file  	main.c
	*	@version V1.0
	*  @date    2022-7-7
	*	@author  ���ͿƼ�	
	*	@brief   LED��˸ʵ��
   *************************************************************************************************
   *  @description
	*
	*	ʵ��ƽ̨������STM32H750XBH6���İ� ���ͺţ�FK750M5-XBH6��
	*	�Ա���ַ��https://shop212360197.taobao.com
	*	QQ����Ⱥ��536665479
	*
>>>>> �ļ�˵����
	*
	*	��ʼ��LED��IO�ڣ�����Ϊ����������������ٶȵȼ�2M��
	*
	************************************************************************************************
***/

#include "stm32h7xx_hal.h"
#include "led.h"  


void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_RCC_LED1_CLK_ENABLE;		// ��ʼ��LED1 GPIOʱ��	


	HAL_GPIO_WritePin(LED1_PORT, LED1_PIN, GPIO_PIN_RESET);		// LED1��������ͣ�������LED1


	GPIO_InitStruct.Pin 		= LED1_PIN;					// LED1����
	GPIO_InitStruct.Mode 	= GPIO_MODE_OUTPUT_PP;	// �������ģʽ
	GPIO_InitStruct.Pull 	= GPIO_NOPULL;				// ��������
	GPIO_InitStruct.Speed 	= GPIO_SPEED_FREQ_LOW;	// ����ģʽ
	HAL_GPIO_Init(LED1_PORT, &GPIO_InitStruct);


}


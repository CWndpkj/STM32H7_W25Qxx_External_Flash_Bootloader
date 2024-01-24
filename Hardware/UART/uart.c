/***
    ************************************************************************************************
    *	@file  	usart.c
    *	@version V1.0
    *  @date    2022-7-7
    *	@author  反客科技
    *	@brief   usart相关函数
   ************************************************************************************************
   *  @description
    *
    *	实验平台：反客STM32H750XBH6核心板 （型号：FK750M5-XBH6）
    *	淘宝地址：https://shop212360197.taobao.com
    *	QQ交流群：536665479
    *
>>>>> 文件说明：
    *
    *  初始化usart引脚，配置波特率等参数
    *
    ************************************************************************************************
***/
#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#include "uart.h"
#include "stm32h7xx_hal.h"
#include "Handlers.h"

int _write (int fd, char *pBuffer, int size)  
{  
    HAL_UART_Transmit(&huart1, (const uint8_t *)pBuffer, size,HAL_MAX_DELAY); // 发送单字节数据
    return size;  
}
/*************************************************************************************************
 *	函 数 名:	HAL_UART_MspInit
 *	入口参数:	huart - UART_HandleTypeDef定义的变量，即表示定义的串口
 *	返 回 值:	无
 *	函数功能:	初始化串口引脚
 *	说    明:	无
 *************************************************************************************************/

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if (huart->Instance == USART1) {
        __HAL_RCC_USART1_CLK_ENABLE(); // 开启 USART1 时钟

        GPIO_USART1_TX_CLK_ENABLE; // 开启 USART1 TX 引脚的 GPIO 时钟
        GPIO_USART1_RX_CLK_ENABLE; // 开启 USART1 RX 引脚的 GPIO 时钟

        GPIO_InitStruct.Pin       = USART1_TX_PIN;             // TX引脚
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;           // 复用推挽输出
        GPIO_InitStruct.Pull      = GPIO_PULLUP;               // 上拉
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH; // 速度等级
        GPIO_InitStruct.Alternate = GPIO_AF7_USART1;           // 复用为USART1
        HAL_GPIO_Init(USART1_TX_PORT, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = USART1_RX_PIN; // RX引脚
        HAL_GPIO_Init(USART1_RX_PORT, &GPIO_InitStruct);
    }
}

/*************************************************************************************************
 *	函 数 名:	USART1_Init
 *	入口参数:	无
 *	返 回 值:	无
 *	函数功能:	初始化串口配置
 *	说    明:	无
 *************************************************************************************************/

void UART1_Init(void)
{
    huart1.Instance                    = USART1;
    huart1.Init.BaudRate               = 115200;
    huart1.Init.WordLength             = UART_WORDLENGTH_8B;
    huart1.Init.StopBits               = UART_STOPBITS_1;
    huart1.Init.Parity                 = UART_PARITY_NONE;
    huart1.Init.Mode                   = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl              = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling           = UART_OVERSAMPLING_16;
    huart1.Init.OneBitSampling         = UART_ONE_BIT_SAMPLE_DISABLE;
    huart1.Init.ClockPrescaler         = UART_PRESCALER_DIV1;
    huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&huart1) != HAL_OK) {
    }
    if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK) {
    }
    if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK) {
    }
    if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK) {
    }
}

#ifdef __cplusplus
}
#endif  // __cplusplus

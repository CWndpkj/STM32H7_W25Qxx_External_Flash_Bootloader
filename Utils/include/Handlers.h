#ifndef __HANDLERS_H
#define __HANDLERS_H
#ifdef STM32H750xx
#include "stm32h7xx_hal.h"
#elif defined(STM32F407xx)
#include "stm32f4xx_hal.h"
#elif defined(STM32F103xB)
#include "stm32f1xx_hal.h"
#endif // 

/**
 * @brief I2C句柄
 */
extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;
extern I2C_HandleTypeDef hi2c3;

/**
 * @brief SPI句柄
 */
extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;
extern SPI_HandleTypeDef hspi3;

/**
 * @brief UART句柄
 */
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart6;

#endif // !__HANDLERS_H
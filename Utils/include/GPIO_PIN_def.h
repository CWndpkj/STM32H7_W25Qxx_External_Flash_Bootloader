#ifndef __GPIO_PIN_DEF
#define __GPIO_PIN_DEF

/**
 * @brief 此文件宏定义本开发板的所有板上外设的引脚对应的GPIO
 * 
 */
#define SW4_Pin GPIO_PIN_1
#define SW4_GPIO_Port GPIOE
#define SW5_Pin GPIO_PIN_2
#define SW5_GPIO_Port GPIOE
#define SW6_Pin GPIO_PIN_3
#define SW6_GPIO_Port GPIOE
#define SW7_Pin GPIO_PIN_4
#define SW7_GPIO_Port GPIOE
#define SW8_Pin GPIO_PIN_5
#define SW8_GPIO_Port GPIOE
#define SW9_Pin GPIO_PIN_6
#define SW9_GPIO_Port GPIOE

#define LED1_Pin GPIO_PIN_13
#define LED1_GPIO_Port GPIOF
#define LED2_Pin GPIO_PIN_14
#define LED2_GPIO_Port GPIOF
#define LED3_Pin GPIO_PIN_15
#define LED3_GPIO_Port GPIOF
#define LED4_Pin GPIO_PIN_7
#define LED4_GPIO_Port GPIOD
#define LED5_Pin GPIO_PIN_10
#define LED5_GPIO_Port GPIOG
#define LED6_Pin GPIO_PIN_11
#define LED6_GPIO_Port GPIOG
#define LED7_Pin GPIO_PIN_13
#define LED7_GPIO_Port GPIOG
#define LED8_Pin GPIO_PIN_15
#define LED8_GPIO_Port GPIOG


/*----------------------------------------------- 引脚配置宏 ------------------------------------------*/

#define QUADSPI_CLK_PIN             GPIO_PIN_10                  // QUADSPI_CLK 引脚
#define QUADSPI_CLK_PORT            GPIOF                        // QUADSPI_CLK 引脚端口
#define QUADSPI_CLK_AF              GPIO_AF9_QUADSPI             // QUADSPI_CLK IO口复用
#define GPIO_QUADSPI_CLK_ENABLE     __HAL_RCC_GPIOF_CLK_ENABLE() // QUADSPI_CLK 引脚时钟

#define QUADSPI_BK1_NCS_PIN         GPIO_PIN_6                   // QUADSPI_BK1_NCS 引脚
#define QUADSPI_BK1_NCS_PORT        GPIOG                        // QUADSPI_BK1_NCS 引脚端口
#define QUADSPI_BK1_NCS_AF          GPIO_AF10_QUADSPI            // QUADSPI_BK1_NCS IO口复用
#define GPIO_QUADSPI_BK1_NCS_ENABLE __HAL_RCC_GPIOG_CLK_ENABLE() // QUADSPI_BK1_NCS 引脚时钟

#define QUADSPI_BK1_IO0_PIN         GPIO_PIN_8                   // QUADSPI_BK1_IO0 引脚
#define QUADSPI_BK1_IO0_PORT        GPIOF                        // QUADSPI_BK1_IO0 引脚端口
#define QUADSPI_BK1_IO0_AF          GPIO_AF10_QUADSPI            // QUADSPI_BK1_IO0 IO口复用
#define GPIO_QUADSPI_BK1_IO0_ENABLE __HAL_RCC_GPIOF_CLK_ENABLE() // QUADSPI_BK1_IO0 引脚时钟

#define QUADSPI_BK1_IO1_PIN         GPIO_PIN_9                   // QUADSPI_BK1_IO1 引脚
#define QUADSPI_BK1_IO1_PORT        GPIOF                        // QUADSPI_BK1_IO1 引脚端口
#define QUADSPI_BK1_IO1_AF          GPIO_AF10_QUADSPI            // QUADSPI_BK1_IO1 IO口复用
#define GPIO_QUADSPI_BK1_IO1_ENABLE __HAL_RCC_GPIOF_CLK_ENABLE() // QUADSPI_BK1_IO1 引脚时钟

#define QUADSPI_BK1_IO2_PIN         GPIO_PIN_7                   // QUADSPI_BK1_IO2 引脚
#define QUADSPI_BK1_IO2_PORT        GPIOF                        // QUADSPI_BK1_IO2 引脚端口
#define QUADSPI_BK1_IO2_AF          GPIO_AF9_QUADSPI             // QUADSPI_BK1_IO2 IO口复用
#define GPIO_QUADSPI_BK1_IO2_ENABLE __HAL_RCC_GPIOF_CLK_ENABLE() // QUADSPI_BK1_IO2 引脚时钟

#define QUADSPI_BK1_IO3_PIN         GPIO_PIN_6                   // QUADSPI_BK1_IO3 引脚
#define QUADSPI_BK1_IO3_PORT        GPIOF                        // QUADSPI_BK1_IO3 引脚端口
#define QUADSPI_BK1_IO3_AF          GPIO_AF9_QUADSPI             // QUADSPI_BK1_IO3 IO口复用
#define GPIO_QUADSPI_BK1_IO3_ENABLE __HAL_RCC_GPIOF_CLK_ENABLE() // QUADSPI_BK1_IO3 引脚时钟

#endif // __GPIO_PIN_DEF

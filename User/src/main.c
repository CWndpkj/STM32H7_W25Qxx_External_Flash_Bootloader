#include "led.h"
#include "sys.h"
#include "uart.h"
#include "w25qxx.h"

#define W25Qxx_Mem_Addr 0x90000000
typedef void ( *pFunction )( void );

pFunction JumpToApplication;

int main( void )
{
    HAL_Init();                        // 初始化HAL库
    Stm32_Clock_Init( 160, 5, 2, 2 );  // 配置系统时钟，主频480MHz
    UART1_Init();                     // USART1初始化
    W25Qxx_init();                     // 初始化W25Q64
    // W25Qxx_enable_qpi_mode();
    W25Qxx_enable_mem_map();  // 配置QSPI为内存映射模式

    SysTick->CTRL = 0;  // 关闭SysTick
    SysTick->LOAD = 0;  // 清零重载值
    SysTick->VAL  = 0;  // 清零计数值

    for ( uint8_t i = 0; i < 8; i++ ) {
        NVIC->ICER[ i ] = 0xFFFFFFFF;
        NVIC->ICPR[ i ] = 0xFFFFFFFF;
    }

    printf( "\r\n*************************\r\n" );
    printf( "W25Q128 Init Success>>>\r\n" );
    printf( "*************************\r\n\r\n" );

    JumpToApplication = ( pFunction )( *( __IO uint32_t* )( W25Qxx_Mem_Addr + 4 ) );  // 设置起始地址
    __set_MSP( *( __IO uint32_t* )W25Qxx_Mem_Addr );                                  // 设置主堆栈指针
    JumpToApplication();                                                              // 执行跳转

    while ( 1 ) {
        LED1_Toggle;
    }
}
#ifndef __W25QXX
#define __W25QXX

#include "stm32h7xx_hal.h"

/**
 * @brief quad SPI(QSPI)采用1线指令，一线或四线地址(视指令而定，指令为quad output的为一线地址，指令为quad IO的为四线地址)，一线或四线数据(视指令而定,支持quad IO或 quad
 * output的指令将通过四个IO进行数据输出),要使支持四线数据输出的指令有效，需要置位QSPI对应的标志位(寄存器bit2)
 *
 *   QPI模式:与QSPI不同，该模式下所有的传输均采用四线模式,要进入该模式,要在开启QSPI模式的基础上，发送0x38指令进入，
 *   发送0xFF退出
 *
 *
 * @note W25Q64FV 支持QPI,W25Q64JV不支持QPI
 */

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

typedef enum Error_code {
    ok                      = 1,
    auto_polling_failed     = 2,
    unrecognized_devideID   = 3,
    write_error             = 4,
    read_error              = 5,
    erase_error             = 6,
    hal_send_command_failed = 7,
    hal_receive_failed      = 8,
    hal_transmit_failed     = 9,
    hal_auto_polling_failed = 10,
    hal_mem_map_failed      = 11,
    unable_to_reach_device  = 12,

} Error_code;

typedef enum Device {
    W25Q80  = 0x13,
    W25Q16  = 0x14,
    W25Q32  = 0x15,
    W25Q64  = 0x16,
    W25Q128 = 0x17,
    W25Q256 = 0x18,
    unknown = 0x00,
} device;

typedef enum Erase_type {
    setor_erase      = 0x20,
    block_erase_32KB = 0x52,
    block_erase_64KB = 0xD8,
    chip_erase       = 0xC7,
} erase_type;
typedef enum Tran_mode {
    QSPI,
    QPI,
} tran_mode;

typedef enum Addr_mode {
    three_bytes,
    four_bytes,
} addr_mode;

typedef enum Instruction {
    // 复位
    c_enable_reset = 0x66,
    c_reset_device = 0x99,

    // 识别操作
    c_read_deviceID         = 0x90,
    c_read_deviceID_dual_IO = 0x92,
    c_read_deviceID_quad_IO = 0x94,
    c_read_jedec_deviceID   = 0x9F,
    c_read_uniqueID         = 0x48,

    // 擦除操作
    c_setor_erase      = 0x20,
    c_block_erase_32KB = 0x52,
    c_block_erase_64KB = 0xD8,
    c_chip_erase       = 0xC7,

    // 写入控制操作
    c_write_enable  = 0x06,
    c_write_disable = 0x04,

    // 状态寄存器操作
    c_volatile_SR_write_enable = 0x50,
    c_read_reg1                = 0x05,
    c_read_reg2                = 0x35,
    c_read_reg3                = 0x15,
    c_write_reg1               = 0x01,
    c_write_reg2               = 0x31,
    c_write_reg3               = 0x11,

    // 读取操作
    c_read_data             = 0x03, // SPI mode Only ,QPI not support
    c_fast_read_data        = 0x0B,
    c_fast_read_dual_output = 0x3B,
    c_fast_read_dual_IO     = 0xBB, // 相比与fast_read_dual_output,在地址输入时也是2lines输入而不是1line
    c_fast_read_quad_output = 0x6B,
    c_fast_read_quad_IO     = 0xEB,
    /***************************ONLY FOR W25Q256**********************************/
    c_read_data_4byte_addr             = 0x13,
    c_fast_read_4byte_addr             = 0x0C,
    c_fast_read_dual_output_4byte_addr = 0x3C,
    c_fast_read_quad_output_4byte_addr = 0x6C,
    c_fast_read_dual_IO_4byte_addr     = 0xBC,
    c_fast_read_quad_IO_4byte_addr     = 0xEC,
    c_word_read_quad_IO                = 0xE7,
    c_octal_word_read_quad_IO          = 0xE3,
    /***********************************************************************/
    // 编程操作
    c_page_program            = 0x02,
    c_quad_input_page_program = 0x32, // QSPI mode only
    // 电源操作
    c_pwr_down         = 0xB9,
    c_release_pwr_down = 0xAB,

    /************************ONLY FOR W25Q256********************************/
    // 模式变更操作
    c_enable_4byte_addr_mode = 0xB7,
    c_exit_4byte_addr_mode   = 0xE9,
    /**************************************************************************/

    /*********************ONLY FOR W25Q128,W25Q256***********************/
    c_set_read_param = 0xC0,
    c_enter_qpi_mode = 0x38,
    c_exit_qpi_mode  = 0xFF,
    /**************************************************************************/
} instruction;

uint8_t W25Qxx_read_reg(uint8_t reg_ID);
uint8_t W25Qxx_volatile_SR_write_enable(void);
uint8_t W25Qxx_write_reg(uint8_t reg_ID, uint8_t reg_value);
uint8_t W25Qxx_write_enable(void);
uint8_t W25Qxx_write_disable(void);
uint8_t W25Qxx_set_write_param(uint8_t param);
uint8_t W25Qxx_erase(erase_type erase_ty, uint32_t addr);
uint8_t W25Qxx_enable_4byte_addr_mode(void);
uint8_t W25Qxx_disable_4byte_addr_mode(void);
uint8_t W25Qxx_pwr_down(void);
uint8_t W25Qxx_wake_up(void);
uint8_t W25Qxx_write_no_check(uint8_t *pBuff, uint32_t Addr, uint32_t length);
uint8_t W25Qxx_write_auto_shift_page(uint8_t *pBuff, uint32_t Addr, uint32_t length);
uint8_t W25Qxx_auto_polling_ready(uint32_t time_out);

uint8_t W25Qxx_init(void);
uint8_t W25Qxx_reset_device(void);
device W25Qxx_read_deviceID(void);
uint8_t W25Qxx_enable_mem_map(void);
uint8_t W25Qxx_disable_mem_map(void);
uint8_t W25Qxx_enable_qpi_mode(void);
uint8_t W25Qxx_disable_qpi_mode(void);
uint8_t W25Qxx_write_len(uint8_t *pBuff, uint32_t Addr, uint32_t length);
uint8_t W25Qxx_read_len(uint8_t *pBuff, uint32_t Addr, uint32_t length);
#endif

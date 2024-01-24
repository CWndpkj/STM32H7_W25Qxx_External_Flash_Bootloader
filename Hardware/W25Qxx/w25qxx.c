#include "w25qxx.h"

QSPI_HandleTypeDef m_QSPI_Handler;
device m_device         = unknown;
Error_code m_error_code = ok;
tran_mode m_tran_mode   = QSPI;

uint8_t W25Qxx_read_reg(uint8_t reg_ID)
{
    QSPI_CommandTypeDef cmd = {0};
    if (reg_ID == 1) {
        cmd.Instruction = (uint32_t)c_read_reg1;
    } else if (reg_ID == 2) {
        cmd.Instruction = (uint32_t)c_read_reg2;
    } else if (reg_ID == 3) {
        cmd.Instruction = (uint32_t)c_read_reg3;
    }
    cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    cmd.DataMode        = QSPI_DATA_1_LINE;
    if (m_tran_mode == QPI) {
        cmd.InstructionMode = QSPI_INSTRUCTION_4_LINES;
        cmd.DataMode        = QSPI_DATA_4_LINES;
    }
    cmd.NbData = 1;
    if (HAL_QSPI_Command(&m_QSPI_Handler, &cmd, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        m_error_code = hal_send_command_failed;
        return 0;
    }

    uint8_t reg_value;
    if (HAL_QSPI_Receive(&m_QSPI_Handler, &reg_value, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        m_error_code = hal_receive_failed;
        return 0;
    }
    return reg_value;
}
uint8_t W25Qxx_volatile_SR_write_enable()
{
    if (!W25Qxx_write_enable()) {
        return 0;
    }
    QSPI_CommandTypeDef cmd = {0};
    cmd.Instruction         = (uint32_t)c_volatile_SR_write_enable;
    cmd.InstructionMode     = QSPI_INSTRUCTION_1_LINE;
    if (m_tran_mode == QPI) {
        cmd.Instruction = QSPI_INSTRUCTION_4_LINES;
    }

    if (HAL_QSPI_Command(&m_QSPI_Handler, &cmd, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        m_error_code = hal_send_command_failed;
        return 0;
    }
    return 1;
}
uint8_t W25Qxx_write_reg(uint8_t reg_ID, uint8_t reg_value)
{
    if (!W25Qxx_write_enable()) {
        return 0;
    }
    QSPI_CommandTypeDef cmd = {0};
    if (reg_ID == 1) {
        cmd.Instruction = (uint32_t)c_write_reg1;
    } else if (reg_ID == 2) {
        cmd.Instruction = (uint32_t)c_write_reg2;
    } else if (reg_ID == 3) {
        cmd.Instruction = (uint32_t)c_write_reg3;
    }
    cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    cmd.DataMode        = QSPI_DATA_1_LINE;
    if (m_tran_mode == QPI) {
        cmd.Instruction = QSPI_INSTRUCTION_4_LINES;
        cmd.DataMode    = QSPI_DATA_4_LINES;
    }
    cmd.NbData = 1;

    if (HAL_QSPI_Command(&m_QSPI_Handler, &cmd, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        m_error_code = hal_send_command_failed;
        return 0;
    }

    if (HAL_QSPI_Transmit(&m_QSPI_Handler, &reg_value, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        m_error_code = hal_transmit_failed;
        return 0;
    }

    return 1;
}
uint8_t W25Qxx_write_enable()
{
    QSPI_CommandTypeDef cmd = {0};
    cmd.Instruction         = (uint32_t)c_write_enable;
    cmd.InstructionMode     = QSPI_INSTRUCTION_1_LINE;
    if (m_tran_mode == QPI) {
        cmd.InstructionMode = QSPI_INSTRUCTION_4_LINES;
    }
    if (HAL_QSPI_Command(&m_QSPI_Handler, &cmd, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        m_error_code = hal_send_command_failed;
        return 0;
    }
    QSPI_CommandTypeDef s_command    = {0};
    QSPI_AutoPollingTypeDef s_config = {0};                        // 轮询比较相关配置参数
    s_config.Match                   = 0x02 | (0x02 << 8);         // 匹配值
    s_config.Mask                    = 0x02 | (0x02 << 8);         // 读状态寄存器1的第1位（只读），WEL写使能标志位，该标志位为1时，代表可以进行写操作
    s_config.MatchMode               = QSPI_MATCH_MODE_AND;        // 与运算
    s_config.StatusBytesSize         = 2;                          // 状态字节数
    s_config.Interval                = 0x10;                       // 轮询间隔
    s_config.AutomaticStop           = QSPI_AUTOMATIC_STOP_ENABLE; // 自动停止模式

    s_command.Instruction     = (uint32_t)c_read_reg1; // 读状态信息寄存器
    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.DataMode        = QSPI_DATA_1_LINE; // 1线数据模式
    if (m_tran_mode == QPI) {
        s_command.DummyCycles     = 2;
        s_command.InstructionMode = QSPI_INSTRUCTION_4_LINES;
        s_command.DataMode        = QSPI_DATA_4_LINES; // 1线数据模式
    }

    // 发送轮询等待命令
    if (HAL_QSPI_AutoPolling(&m_QSPI_Handler, &s_command, &s_config, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        m_error_code = hal_auto_polling_failed;
        return 0;
    }
    return 1; // 通信正常结束
}
uint8_t W25Qxx_write_disable()
{
    QSPI_CommandTypeDef cmd = {0};
    cmd.Instruction         = (uint32_t)c_write_disable;
    cmd.InstructionMode     = QSPI_INSTRUCTION_1_LINE;
    cmd.SIOOMode            = QSPI_SIOO_INST_EVERY_CMD;
    if (m_tran_mode == QPI) {
        cmd.InstructionMode = QSPI_INSTRUCTION_4_LINES;
    }
    if (HAL_QSPI_Command(&m_QSPI_Handler, &cmd, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        m_error_code = hal_send_command_failed;
        return 0;
    }
    QSPI_AutoPollingTypeDef s_config = {0};                        // 轮询比较相关配置参数
    s_config.Match                   = 0x00;                       // 匹配值
    s_config.Mask                    = 0x02;                       // 读状态寄存器1的第1位（只读），WEL写使能标志位，该标志位为1时，代表可以进行写操作
    s_config.MatchMode               = QSPI_MATCH_MODE_AND;        // 与运算
    s_config.StatusBytesSize         = 1;                          // 状态字节数
    s_config.Interval                = 0x10;                       // 轮询间隔
    s_config.AutomaticStop           = QSPI_AUTOMATIC_STOP_ENABLE; // 自动停止模式

    cmd.Instruction = 0x05;             // 读状态信息寄存器
    cmd.DataMode    = QSPI_DATA_1_LINE; // 1线数据模式
    if (m_tran_mode == QPI) {
        cmd.DataMode = QSPI_DATA_4_LINES; // 1线数据模式
    }
    cmd.NbData = 1; // 数据长度
    cmd.NbData = 1; // 数据长度

    // 发送轮询等待命令
    if (HAL_QSPI_AutoPolling(&m_QSPI_Handler, &cmd, &s_config, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        m_error_code = hal_auto_polling_failed;
        return 0;
    }
    return 1; // 通信正常结束
}

uint8_t W25Qxx_set_write_param(uint8_t param)
{
    QSPI_CommandTypeDef cmd = {0};
    cmd.Instruction         = (uint32_t)c_set_read_param;
    cmd.InstructionMode     = QSPI_INSTRUCTION_1_LINE;
    cmd.DataMode            = QSPI_DATA_1_LINE;
    cmd.NbData              = 1;

    if (m_tran_mode == QPI) {
        cmd.InstructionMode = QSPI_INSTRUCTION_4_LINES;
        cmd.DataMode        = QSPI_DATA_4_LINES;
    }
    if (HAL_QSPI_Command(&m_QSPI_Handler, &cmd, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        m_error_code = hal_send_command_failed;
        return 0;
    }
    if (HAL_QSPI_Transmit(&m_QSPI_Handler, &param, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        m_error_code = hal_transmit_failed;
        return 0;
    }
    return W25Qxx_auto_polling_ready(HAL_QSPI_TIMEOUT_DEFAULT_VALUE);
}

uint8_t W25Qxx_erase(erase_type erase_type, uint32_t Addr)
{
    if (!W25Qxx_write_enable()) {
        return 0;
    }
    QSPI_CommandTypeDef cmd = {0};
    cmd.Instruction         = (uint32_t)erase_type;
    cmd.InstructionMode     = QSPI_INSTRUCTION_1_LINE;
    cmd.AddressMode         = QSPI_ADDRESS_1_LINE;
    cmd.AddressSize         = QSPI_ADDRESS_24_BITS;
    cmd.Address             = Addr;
    if (m_tran_mode == QPI) {
        cmd.AddressMode     = QSPI_ADDRESS_4_LINES;
        cmd.InstructionMode = QSPI_INSTRUCTION_4_LINES;
    }
    if (erase_type == chip_erase) {
        cmd.AddressMode = QSPI_ADDRESS_NONE;
    }
    if (HAL_QSPI_Command(&m_QSPI_Handler, &cmd, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        m_error_code = hal_send_command_failed;
        return 0;
    }
    return W25Qxx_auto_polling_ready(HAL_MAX_DELAY);
}
uint8_t W25Qxx_enable_4byte_addr_mode()
{
    if (m_device != W25Q256) {
        return 0;
    }
    QSPI_CommandTypeDef cmd = {0};
    cmd.Instruction         = (uint32_t)c_enable_4byte_addr_mode;
    cmd.InstructionMode     = QSPI_INSTRUCTION_1_LINE;
    if (m_tran_mode == QPI) {
        cmd.InstructionMode = QSPI_INSTRUCTION_4_LINES;
    }
    if (HAL_QSPI_Command(&m_QSPI_Handler, &cmd, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        m_error_code = hal_send_command_failed;
        return 0;
    }
    return W25Qxx_auto_polling_ready(HAL_QSPI_TIMEOUT_DEFAULT_VALUE);
}

uint8_t W25Qxx_disable_4byte_addr_mode()
{
    if (m_device != W25Q256) {
        return 0;
    }
    QSPI_CommandTypeDef cmd = {0};
    cmd.Instruction         = (uint32_t)c_exit_4byte_addr_mode;
    cmd.InstructionMode     = QSPI_INSTRUCTION_1_LINE;
    if (m_tran_mode == QPI) {
        cmd.InstructionMode = QSPI_INSTRUCTION_4_LINES;
    }
    if (HAL_QSPI_Command(&m_QSPI_Handler, &cmd, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        m_error_code = hal_send_command_failed;
        return 0;
    }
    return W25Qxx_auto_polling_ready(HAL_QSPI_TIMEOUT_DEFAULT_VALUE);
}
uint8_t W25Qxx_pwr_down()
{
    QSPI_CommandTypeDef cmd = {0};
    cmd.Instruction         = (uint32_t)c_pwr_down;
    cmd.InstructionMode     = QSPI_INSTRUCTION_1_LINE;
    if (m_tran_mode == QPI) {
        cmd.InstructionMode = QSPI_INSTRUCTION_4_LINES;
    }
    if (HAL_QSPI_Command(&m_QSPI_Handler, &cmd, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        m_error_code = hal_send_command_failed;
        return 0;
    }
    return W25Qxx_auto_polling_ready(HAL_QSPI_TIMEOUT_DEFAULT_VALUE);
}
uint8_t W25Qxx_wake_up()
{
    QSPI_CommandTypeDef cmd = {0};
    cmd.Instruction         = (uint32_t)c_release_pwr_down;
    cmd.InstructionMode     = QSPI_INSTRUCTION_1_LINE;
    if (m_tran_mode == QPI) {
        cmd.InstructionMode = QSPI_INSTRUCTION_4_LINES;
    }
    if (HAL_QSPI_Command(&m_QSPI_Handler, &cmd, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        m_error_code = hal_send_command_failed;
        return 0;
    }
    return W25Qxx_auto_polling_ready(HAL_QSPI_TIMEOUT_DEFAULT_VALUE);
}
uint8_t W25Qxx_write_no_check(uint8_t *pBuff, uint32_t Addr, uint32_t length)
{
    if (!W25Qxx_write_enable()) {
        return 0;
    }
    QSPI_CommandTypeDef cmd = {0};
    /**
     * @brief 此处采用四输入的写入模式,指令和地址均为1线，数据为4线(SPI mode only)
     */
    cmd.Instruction     = (uint32_t)c_quad_input_page_program;
    cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    cmd.AddressMode     = QSPI_ADDRESS_1_LINE;
    cmd.AddressSize     = QSPI_ADDRESS_24_BITS;
    cmd.DataMode        = QSPI_DATA_4_LINES;
    if (m_tran_mode == QPI) {
        cmd.Instruction     = (uint32_t)c_page_program;
        cmd.InstructionMode = QSPI_INSTRUCTION_4_LINES;
        cmd.AddressMode     = QSPI_ADDRESS_4_LINES;
        cmd.DataMode        = QSPI_DATA_4_LINES;
    }
    cmd.Address = Addr;
    cmd.NbData  = length;
    if (HAL_QSPI_Command(&m_QSPI_Handler, &cmd, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        m_error_code = hal_send_command_failed;
        return 0;
    }
    if (HAL_QSPI_Transmit(&m_QSPI_Handler, pBuff, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        m_error_code = hal_transmit_failed;
        return 0;
    }
    return W25Qxx_auto_polling_ready(HAL_QSPI_TIMEOUT_DEFAULT_VALUE);
}

uint8_t W25Qxx_write_auto_shift_page(uint8_t *pBuff, uint32_t Addr, uint32_t length)
{
    uint32_t page_offset = Addr % 256;
    uint32_t page_num    = Addr / 256;
    uint32_t page_remain = 256 - page_offset;
    while (1) {
        if (length <= page_remain) {
            if (!W25Qxx_write_no_check(pBuff, Addr, length)) {
                m_error_code = write_error;
            }
            break;
        } else {
            if (!W25Qxx_write_no_check(pBuff, Addr, page_remain)) {
                m_error_code = write_error;
            }
            // 更新数据
            Addr += page_remain;
            pBuff += page_remain;
            length -= page_remain;
            page_offset = Addr % 256;
            page_num    = Addr / 256;
            page_remain = 256 - page_offset;
        }
    }
    return 1;
}

uint8_t W25Qxx_auto_polling_ready(uint32_t time_out)
{
    QSPI_CommandTypeDef cmd     = {0};
    QSPI_AutoPollingTypeDef cfg = {0};
    cmd.Instruction             = (uint32_t)c_read_reg1;
    cmd.InstructionMode         = QSPI_INSTRUCTION_1_LINE;
    cmd.DataMode                = QSPI_DATA_1_LINE;
    if (m_tran_mode == QPI) {
        /**
         * @note 在使用轮询模式校验操作是否完成时，需要加适当的空周期,
         * 否则会导致auto_polling失败
         */
        //**************************************************************
        cmd.DummyCycles = 2;
        //***************************************************************
        cmd.InstructionMode = QSPI_INSTRUCTION_4_LINES;
        cmd.DataMode        = QSPI_DATA_4_LINES;
    }

    cfg.Match           = 0x0;
    cfg.Mask            = 0x01 | (0x01 << 8);
    cfg.MatchMode       = QSPI_MATCH_MODE_AND;
    cfg.StatusBytesSize = 2;
    cfg.Interval        = 0x10;
    cfg.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;

    if (HAL_QSPI_AutoPolling(&m_QSPI_Handler, &cmd, &cfg, time_out) != HAL_OK) {
        m_error_code = hal_auto_polling_failed;
        return 0;
    }
    return 1;
}
void HAL_QSPI_MspInit(QSPI_HandleTypeDef *hqspi)
{
    if (hqspi->Instance == QUADSPI) {
        __HAL_RCC_QSPI_CLK_ENABLE(); // 使能QSPI时钟

        __HAL_RCC_QSPI_FORCE_RESET(); // 复位QSPI
        __HAL_RCC_QSPI_RELEASE_RESET();

        GPIO_QUADSPI_CLK_ENABLE;     // 使能 QUADSPI_CLK IO口时钟
        GPIO_QUADSPI_BK1_NCS_ENABLE; // 使能 QUADSPI_BK1_NCS IO口时钟
        GPIO_QUADSPI_BK1_IO0_ENABLE; // 使能 QUADSPI_BK1_IO0 IO口时钟
        GPIO_QUADSPI_BK1_IO1_ENABLE; // 使能 QUADSPI_BK1_IO1 IO口时钟
        GPIO_QUADSPI_BK1_IO2_ENABLE; // 使能 QUADSPI_BK1_IO2 IO口时钟
        GPIO_QUADSPI_BK1_IO3_ENABLE; // 使能 QUADSPI_BK1_IO3 IO口时钟

        /******************************************************
        PG6     ------> QUADSPI_BK1_NCS
        PF6     ------> QUADSPI_BK1_IO3
        PF7     ------> QUADSPI_BK1_IO2
        PF8     ------> QUADSPI_BK1_IO0
        PF10     ------> QUADSPI_CLK
        PF9     ------> QUADSPI_BK1_IO1
        *******************************************************/
        GPIO_InitTypeDef GPIO_InitStruct = {0};
        GPIO_InitStruct.Mode             = GPIO_MODE_AF_PP;           // 复用推挽输出模式
        GPIO_InitStruct.Pull             = GPIO_NOPULL;               // 无上下拉
        GPIO_InitStruct.Speed            = GPIO_SPEED_FREQ_VERY_HIGH; // 超高速IO口速度

        GPIO_InitStruct.Pin       = QUADSPI_CLK_PIN;       // QUADSPI_CLK 引脚
        GPIO_InitStruct.Alternate = QUADSPI_CLK_AF;        // QUADSPI_CLK 复用
        HAL_GPIO_Init(QUADSPI_CLK_PORT, &GPIO_InitStruct); // 初始化 QUADSPI_CLK 引脚

        GPIO_InitStruct.Pin       = QUADSPI_BK1_NCS_PIN;       // QUADSPI_BK1_NCS 引脚
        GPIO_InitStruct.Alternate = QUADSPI_BK1_NCS_AF;        // QUADSPI_BK1_NCS 复用
        HAL_GPIO_Init(QUADSPI_BK1_NCS_PORT, &GPIO_InitStruct); // 初始化 QUADSPI_BK1_NCS 引脚

        GPIO_InitStruct.Pin       = QUADSPI_BK1_IO0_PIN;       // QUADSPI_BK1_IO0 引脚
        GPIO_InitStruct.Alternate = QUADSPI_BK1_IO0_AF;        // QUADSPI_BK1_IO0 复用
        HAL_GPIO_Init(QUADSPI_BK1_IO0_PORT, &GPIO_InitStruct); // 初始化 QUADSPI_BK1_IO0 引脚

        GPIO_InitStruct.Pin       = QUADSPI_BK1_IO1_PIN;       // QUADSPI_BK1_IO1 引脚
        GPIO_InitStruct.Alternate = QUADSPI_BK1_IO1_AF;        // QUADSPI_BK1_IO1 复用
        HAL_GPIO_Init(QUADSPI_BK1_IO1_PORT, &GPIO_InitStruct); // 初始化 QUADSPI_BK1_IO1 引脚

        GPIO_InitStruct.Pin       = QUADSPI_BK1_IO2_PIN;       // QUADSPI_BK1_IO2 引脚
        GPIO_InitStruct.Alternate = QUADSPI_BK1_IO2_AF;        // QUADSPI_BK1_IO2 复用
        HAL_GPIO_Init(QUADSPI_BK1_IO2_PORT, &GPIO_InitStruct); // 初始化 QUADSPI_BK1_IO2 引脚

        GPIO_InitStruct.Pin       = QUADSPI_BK1_IO3_PIN;       // QUADSPI_BK1_IO3 引脚
        GPIO_InitStruct.Alternate = QUADSPI_BK1_IO3_AF;        // QUADSPI_BK1_IO3 复用
        HAL_GPIO_Init(QUADSPI_BK1_IO3_PORT, &GPIO_InitStruct); // 初始化 QUADSPI_BK1_IO3 引脚
    }
}

uint8_t W25Qxx_init()
{
    __HAL_RCC_QSPI_FORCE_RESET(); // 复位QSPI
    __HAL_RCC_QSPI_RELEASE_RESET();

    /*在某些场合，例如用作下载算法时，需要手动清零句柄等参数，否则会工作不正常*/
    uint32_t i;
    char *p;

    /* 此处参考安富莱的代码，大家可以去学习安富莱的教程，非常专业 */
    p = (char *)&m_QSPI_Handler;
    for (i = 0; i < sizeof(m_QSPI_Handler); i++) {
        *p++ = 0;
    }
    m_QSPI_Handler.Instance = QUADSPI;
    HAL_QSPI_DeInit(&m_QSPI_Handler); // 复位QSPI
    /********************/

    m_QSPI_Handler.Instance                = QUADSPI;                        // QSPI实例
    m_QSPI_Handler.Init.ClockPrescaler     = 1;                              // 时钟预分频
    m_QSPI_Handler.Init.FifoThreshold      = 32;                             // FIFO阈值
    m_QSPI_Handler.Init.SampleShifting     = QSPI_SAMPLE_SHIFTING_HALFCYCLE; // 采样移位
    m_QSPI_Handler.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_5_CYCLE;      // 片选高电平时间
    m_QSPI_Handler.Init.DualFlash          = QSPI_DUALFLASH_DISABLE;         // 不使用双闪存
    m_QSPI_Handler.Init.FlashID            = QSPI_FLASH_ID_1;                // 使用第一个闪存
    m_QSPI_Handler.Init.ClockMode          = QSPI_CLOCK_MODE_3;              // SPI模式3
    m_QSPI_Handler.Init.FlashSize          = POSITION_VAL(0X1000000) - 1;    // 闪存大小
    HAL_QSPI_Init(&m_QSPI_Handler);                                          // 初始化QSPI

    if (!W25Qxx_disable_qpi_mode()) {
        m_error_code = unable_to_reach_device;
        return 0;
    }

    if (!W25Qxx_reset_device()) {
        m_error_code = unable_to_reach_device;
        return 0;
    }

    if (!W25Qxx_auto_polling_ready(HAL_QSPI_TIMEOUT_DEFAULT_VALUE)) {
        m_error_code = auto_polling_failed;
        return 0;
    }

    m_device = W25Qxx_read_deviceID();
    if (m_device == unknown) {
        m_error_code = unrecognized_devideID;
        return 0;
    }
    return 1;
}

uint8_t W25Qxx_reset_device()
{
    QSPI_CommandTypeDef cmd = {0};
    cmd.Instruction         = (uint32_t)c_enable_reset;
    cmd.InstructionMode     = QSPI_INSTRUCTION_1_LINE;
    if (m_tran_mode == QPI) {
        cmd.Instruction = QSPI_INSTRUCTION_4_LINES;
    }
    if (HAL_QSPI_Command(&m_QSPI_Handler, &cmd, 100) != HAL_OK) {
        m_error_code = hal_send_command_failed;
        return 0;
    }

    cmd.Instruction = (uint32_t)c_reset_device;
    if (HAL_QSPI_Command(&m_QSPI_Handler, &cmd, 100) != HAL_OK) {
        m_error_code = hal_send_command_failed;
        return 0;
    }
    // reset后默认运行在QSPI
    m_tran_mode = QSPI;
    return 1;
}

device W25Qxx_read_deviceID()
{
    QSPI_CommandTypeDef cmd = {0};
    cmd.Instruction         = (uint32_t)c_release_pwr_down;
    cmd.InstructionMode     = QSPI_INSTRUCTION_1_LINE;
    cmd.DataMode            = QSPI_DATA_1_LINE;
    cmd.DummyCycles         = 24;
    if (m_tran_mode == QPI) {
        cmd.InstructionMode = QSPI_INSTRUCTION_4_LINES;
        // cmd.AlternateByteMode  = QSPI_ALTERNATE_BYTES_4_LINES;
        // cmd.AlternateBytes     = 0x00;
        // cmd.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS;
        // cmd.DummyCycles        = 4;
        cmd.DummyCycles = 8;
        cmd.DataMode    = QSPI_DATA_4_LINES;
    }
    // cmd.AddressSize      = QSPI_ADDRESS_24_BITS;
    cmd.NbData = 1; // 读取2个字节

    if (HAL_QSPI_Command(&m_QSPI_Handler, &cmd, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return unknown;
    }

    uint8_t buff;
    if (HAL_QSPI_Receive(&m_QSPI_Handler, &buff, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return unknown;
    }
    m_device = (device)buff;
    return m_device;

    // QSPI_CommandTypeDef cmd = { 0 };
    // cmd.InstructionMode     = QSPI_INSTRUCTION_1_LINE;
    // cmd.Instruction         = ( uint32_t )Instruction::read_deviceID;
    // cmd.AddressMode         = QSPI_ADDRESS_1_LINE;
    // cmd.DataMode            = QSPI_DATA_1_LINE;
    // cmd.AddressSize         = QSPI_ADDRESS_24_BITS;
    // cmd.Address             = 0;
    // cmd.NbData              = 2;  // 读取2个字节

    // if ( HAL_QSPI_Command( &m_QSPI_Handler, &cmd, HAL_QSPI_TIMEOUT_DEFAULT_VALUE ) != HAL_OK ) {
    //     m_error_code = Error_code::hal_send_command_failed;
    //     printf( "error in read_deviceID :hal_send_command_failed\r\n" );
    //     return Device::unknown;
    // }

    // uint8_t buff[ 2 ];
    // if ( HAL_QSPI_Receive( &m_QSPI_Handler, buff, HAL_QSPI_TIMEOUT_DEFAULT_VALUE ) != HAL_OK ) {
    //     m_error_code = Error_code::hal_receive_failed;
    //     printf( "error in read_deviceID :hal_receive_failed\r\n" );
    //     return Device::unknown;
    // }
    // m_device = ( Device )( buff[ 0 ] << 8 | buff[ 1 ] );
    // return m_device;
}
uint8_t W25Qxx_enable_mem_map()
{
    W25Qxx_set_write_param(0x20);
    QSPI_CommandTypeDef cmd          = {0};
    QSPI_MemoryMappedTypeDef mem_map = {0};
    cmd.Instruction                  = (uint32_t)c_fast_read_quad_IO;
    cmd.InstructionMode              = QSPI_INSTRUCTION_1_LINE;
    cmd.AddressMode                  = QSPI_ADDRESS_4_LINES;
    cmd.AddressSize                  = QSPI_ADDRESS_24_BITS;
    cmd.DataMode                     = QSPI_DATA_4_LINES;
    cmd.AlternateByteMode            = QSPI_ALTERNATE_BYTES_4_LINES;
    cmd.AlternateBytes               = 0x00;
    cmd.AlternateBytesSize           = QSPI_ALTERNATE_BYTES_8_BITS;
    cmd.DummyCycles                  = 4;
    // cmd.SIOOMode                     = QSPI_SIOO_INST_ONLY_FIRST_CMD;
    if (m_tran_mode == QPI) {
        cmd.InstructionMode = QSPI_INSTRUCTION_4_LINES;
        cmd.DummyCycles     = 4;
    }
    mem_map.TimeOutActivation = QSPI_TIMEOUT_COUNTER_DISABLE;
    mem_map.TimeOutPeriod     = 0;
    if (HAL_QSPI_MemoryMapped(&m_QSPI_Handler, &cmd, &mem_map) != HAL_OK) {
        m_error_code = hal_mem_map_failed;
        return 0;
    }
    return 1;
}
uint8_t W25Qxx_disable_mem_map()
{
    return HAL_QSPI_Abort(&m_QSPI_Handler);
}

uint8_t W25Qxx_enable_qpi_mode()
{
    uint8_t reg_value = W25Qxx_read_reg(2);
    if ((reg_value & 0x02) == 0) {
        reg_value |= 0x02;
        W25Qxx_volatile_SR_write_enable();
        W25Qxx_write_reg(2, reg_value);
    }
    QSPI_CommandTypeDef cmd = {0};
    cmd.InstructionMode     = QSPI_INSTRUCTION_1_LINE;
    cmd.Instruction         = (uint32_t)c_enter_qpi_mode;
    if (HAL_QSPI_Command(&m_QSPI_Handler, &cmd, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        m_error_code = hal_send_command_failed;
        return 0;
    }
    m_tran_mode = QPI;
    return 1;
}
uint8_t W25Qxx_disable_qpi_mode()
{
    QSPI_CommandTypeDef cmd = {0};
    cmd.AddressMode         = QSPI_ADDRESS_NONE;
    cmd.AlternateByteMode   = QSPI_ALTERNATE_BYTES_NONE;
    cmd.DataMode            = QSPI_DATA_NONE;
    cmd.InstructionMode     = QSPI_INSTRUCTION_4_LINES;
    cmd.Instruction         = (uint32_t)c_exit_qpi_mode;
    cmd.DdrMode             = QSPI_DDR_MODE_DISABLE;
    cmd.SIOOMode            = QSPI_SIOO_INST_EVERY_CMD;
    if (HAL_QSPI_Command(&m_QSPI_Handler, &cmd, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        m_error_code = hal_send_command_failed;
        return 0;
    }
    m_tran_mode = QSPI;
    return 1;
}

uint8_t W25Qxx_write_len(uint8_t *pBuff, uint32_t Addr, uint32_t length)
{
    uint32_t sector_offset = Addr % 4096;          // 地址在扇区内的偏移量
    uint32_t sector_pos    = Addr / 4096;          // 地址所在扇区位置
    uint32_t sector_remain = 4096 - sector_offset; // 地址所在扇区剩余的大小

    uint8_t buff[4096];
    while (1) {
        // 读取要写入扇区的数据
        if (!W25Qxx_read_len(buff, Addr - sector_offset, 4096)) {
            m_error_code = read_error;
            return 0;
        }
        if (length <= sector_remain) { // 一个扇区可以写完
            if (!W25Qxx_erase(setor_erase, Addr - sector_offset)) {
                m_error_code = erase_error;
                return 0;
            }
            // 修改数组
            for (uint32_t i = sector_offset; i < length + sector_offset; i++) {
                buff[i] = pBuff[i - sector_offset];
            }
            if (!W25Qxx_write_auto_shift_page(buff, Addr - sector_offset, 4096)) {
                m_error_code = write_error;
                return 0;
            }
            break;
        } else { // 需要写到下一扇区
            // 擦除芯片上的数据
            if (!W25Qxx_erase(setor_erase, Addr - sector_offset)) {
                m_error_code = erase_error;
                return 0;
            }
            for (uint32_t i = sector_offset; i < length + sector_offset; i++) {
                buff[i] = pBuff[i - sector_offset];
            }
            if (!W25Qxx_write_auto_shift_page(buff, Addr - sector_offset, 4096)) {
                m_error_code = write_error;
                return 0;
            }
            // 更新数据
            Addr += sector_remain;
            length -= sector_remain;
            pBuff += sector_remain;
            // 重新计算
            sector_offset = Addr % 4096;
            sector_pos    = Addr / 4096;
            sector_remain = 4096 - sector_pos;
        }
    }
    return 1;
}
uint8_t W25Qxx_read_len(uint8_t *pBuff, uint32_t Addr, uint32_t length)
{
    QSPI_CommandTypeDef cmd = {0};
    cmd.Instruction         = (uint32_t)c_fast_read_quad_IO;
    cmd.InstructionMode     = QSPI_INSTRUCTION_1_LINE;
    cmd.AddressMode         = QSPI_ADDRESS_4_LINES;
    cmd.DataMode            = QSPI_DATA_4_LINES;
    cmd.AlternateByteMode   = QSPI_ALTERNATE_BYTES_4_LINES;
    // 该处的cmd.AlternateBytes 若设置为0x20，表示连续读取模式，即下一次不需要再发送0xEB指令，直接读取数据即可
    // 但是会导致下一次发送其他指令时，第一次发送的指令为无效
    // 通常赋值为0以免影响下一次发送的指令
    cmd.AlternateBytes     = 0x00;
    cmd.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS;
    cmd.Address            = Addr;
    cmd.AddressSize        = QSPI_ADDRESS_24_BITS;
    cmd.DummyCycles        = 4;
    if (m_tran_mode == QPI) {
        cmd.InstructionMode = QSPI_INSTRUCTION_4_LINES;
        cmd.DummyCycles     = 0;
    }
    cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
    cmd.NbData   = length;

    if (HAL_QSPI_Command(&m_QSPI_Handler, &cmd, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        m_error_code = hal_send_command_failed;
        return 0;
    }
    if (HAL_QSPI_Receive(&m_QSPI_Handler, pBuff, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        m_error_code = hal_receive_failed;
        return 0;
    }
    return 1;
}

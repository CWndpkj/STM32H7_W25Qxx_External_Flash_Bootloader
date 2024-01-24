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
    QSPI_AutoPollingTypeDef s_config = {0};                        // ��ѯ�Ƚ�������ò���
    s_config.Match                   = 0x02 | (0x02 << 8);         // ƥ��ֵ
    s_config.Mask                    = 0x02 | (0x02 << 8);         // ��״̬�Ĵ���1�ĵ�1λ��ֻ������WELдʹ�ܱ�־λ���ñ�־λΪ1ʱ��������Խ���д����
    s_config.MatchMode               = QSPI_MATCH_MODE_AND;        // ������
    s_config.StatusBytesSize         = 2;                          // ״̬�ֽ���
    s_config.Interval                = 0x10;                       // ��ѯ���
    s_config.AutomaticStop           = QSPI_AUTOMATIC_STOP_ENABLE; // �Զ�ֹͣģʽ

    s_command.Instruction     = (uint32_t)c_read_reg1; // ��״̬��Ϣ�Ĵ���
    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.DataMode        = QSPI_DATA_1_LINE; // 1������ģʽ
    if (m_tran_mode == QPI) {
        s_command.DummyCycles     = 2;
        s_command.InstructionMode = QSPI_INSTRUCTION_4_LINES;
        s_command.DataMode        = QSPI_DATA_4_LINES; // 1������ģʽ
    }

    // ������ѯ�ȴ�����
    if (HAL_QSPI_AutoPolling(&m_QSPI_Handler, &s_command, &s_config, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        m_error_code = hal_auto_polling_failed;
        return 0;
    }
    return 1; // ͨ����������
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
    QSPI_AutoPollingTypeDef s_config = {0};                        // ��ѯ�Ƚ�������ò���
    s_config.Match                   = 0x00;                       // ƥ��ֵ
    s_config.Mask                    = 0x02;                       // ��״̬�Ĵ���1�ĵ�1λ��ֻ������WELдʹ�ܱ�־λ���ñ�־λΪ1ʱ��������Խ���д����
    s_config.MatchMode               = QSPI_MATCH_MODE_AND;        // ������
    s_config.StatusBytesSize         = 1;                          // ״̬�ֽ���
    s_config.Interval                = 0x10;                       // ��ѯ���
    s_config.AutomaticStop           = QSPI_AUTOMATIC_STOP_ENABLE; // �Զ�ֹͣģʽ

    cmd.Instruction = 0x05;             // ��״̬��Ϣ�Ĵ���
    cmd.DataMode    = QSPI_DATA_1_LINE; // 1������ģʽ
    if (m_tran_mode == QPI) {
        cmd.DataMode = QSPI_DATA_4_LINES; // 1������ģʽ
    }
    cmd.NbData = 1; // ���ݳ���
    cmd.NbData = 1; // ���ݳ���

    // ������ѯ�ȴ�����
    if (HAL_QSPI_AutoPolling(&m_QSPI_Handler, &cmd, &s_config, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        m_error_code = hal_auto_polling_failed;
        return 0;
    }
    return 1; // ͨ����������
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
     * @brief �˴������������д��ģʽ,ָ��͵�ַ��Ϊ1�ߣ�����Ϊ4��(SPI mode only)
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
            // ��������
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
         * @note ��ʹ����ѯģʽУ������Ƿ����ʱ����Ҫ���ʵ��Ŀ�����,
         * ����ᵼ��auto_pollingʧ��
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
        __HAL_RCC_QSPI_CLK_ENABLE(); // ʹ��QSPIʱ��

        __HAL_RCC_QSPI_FORCE_RESET(); // ��λQSPI
        __HAL_RCC_QSPI_RELEASE_RESET();

        GPIO_QUADSPI_CLK_ENABLE;     // ʹ�� QUADSPI_CLK IO��ʱ��
        GPIO_QUADSPI_BK1_NCS_ENABLE; // ʹ�� QUADSPI_BK1_NCS IO��ʱ��
        GPIO_QUADSPI_BK1_IO0_ENABLE; // ʹ�� QUADSPI_BK1_IO0 IO��ʱ��
        GPIO_QUADSPI_BK1_IO1_ENABLE; // ʹ�� QUADSPI_BK1_IO1 IO��ʱ��
        GPIO_QUADSPI_BK1_IO2_ENABLE; // ʹ�� QUADSPI_BK1_IO2 IO��ʱ��
        GPIO_QUADSPI_BK1_IO3_ENABLE; // ʹ�� QUADSPI_BK1_IO3 IO��ʱ��

        /******************************************************
        PG6     ------> QUADSPI_BK1_NCS
        PF6     ------> QUADSPI_BK1_IO3
        PF7     ------> QUADSPI_BK1_IO2
        PF8     ------> QUADSPI_BK1_IO0
        PF10     ------> QUADSPI_CLK
        PF9     ------> QUADSPI_BK1_IO1
        *******************************************************/
        GPIO_InitTypeDef GPIO_InitStruct = {0};
        GPIO_InitStruct.Mode             = GPIO_MODE_AF_PP;           // �����������ģʽ
        GPIO_InitStruct.Pull             = GPIO_NOPULL;               // ��������
        GPIO_InitStruct.Speed            = GPIO_SPEED_FREQ_VERY_HIGH; // ������IO���ٶ�

        GPIO_InitStruct.Pin       = QUADSPI_CLK_PIN;       // QUADSPI_CLK ����
        GPIO_InitStruct.Alternate = QUADSPI_CLK_AF;        // QUADSPI_CLK ����
        HAL_GPIO_Init(QUADSPI_CLK_PORT, &GPIO_InitStruct); // ��ʼ�� QUADSPI_CLK ����

        GPIO_InitStruct.Pin       = QUADSPI_BK1_NCS_PIN;       // QUADSPI_BK1_NCS ����
        GPIO_InitStruct.Alternate = QUADSPI_BK1_NCS_AF;        // QUADSPI_BK1_NCS ����
        HAL_GPIO_Init(QUADSPI_BK1_NCS_PORT, &GPIO_InitStruct); // ��ʼ�� QUADSPI_BK1_NCS ����

        GPIO_InitStruct.Pin       = QUADSPI_BK1_IO0_PIN;       // QUADSPI_BK1_IO0 ����
        GPIO_InitStruct.Alternate = QUADSPI_BK1_IO0_AF;        // QUADSPI_BK1_IO0 ����
        HAL_GPIO_Init(QUADSPI_BK1_IO0_PORT, &GPIO_InitStruct); // ��ʼ�� QUADSPI_BK1_IO0 ����

        GPIO_InitStruct.Pin       = QUADSPI_BK1_IO1_PIN;       // QUADSPI_BK1_IO1 ����
        GPIO_InitStruct.Alternate = QUADSPI_BK1_IO1_AF;        // QUADSPI_BK1_IO1 ����
        HAL_GPIO_Init(QUADSPI_BK1_IO1_PORT, &GPIO_InitStruct); // ��ʼ�� QUADSPI_BK1_IO1 ����

        GPIO_InitStruct.Pin       = QUADSPI_BK1_IO2_PIN;       // QUADSPI_BK1_IO2 ����
        GPIO_InitStruct.Alternate = QUADSPI_BK1_IO2_AF;        // QUADSPI_BK1_IO2 ����
        HAL_GPIO_Init(QUADSPI_BK1_IO2_PORT, &GPIO_InitStruct); // ��ʼ�� QUADSPI_BK1_IO2 ����

        GPIO_InitStruct.Pin       = QUADSPI_BK1_IO3_PIN;       // QUADSPI_BK1_IO3 ����
        GPIO_InitStruct.Alternate = QUADSPI_BK1_IO3_AF;        // QUADSPI_BK1_IO3 ����
        HAL_GPIO_Init(QUADSPI_BK1_IO3_PORT, &GPIO_InitStruct); // ��ʼ�� QUADSPI_BK1_IO3 ����
    }
}

uint8_t W25Qxx_init()
{
    __HAL_RCC_QSPI_FORCE_RESET(); // ��λQSPI
    __HAL_RCC_QSPI_RELEASE_RESET();

    /*��ĳЩ���ϣ��������������㷨ʱ����Ҫ�ֶ��������Ȳ���������Ṥ��������*/
    uint32_t i;
    char *p;

    /* �˴��ο��������Ĵ��룬��ҿ���ȥѧϰ�������Ľ̳̣��ǳ�רҵ */
    p = (char *)&m_QSPI_Handler;
    for (i = 0; i < sizeof(m_QSPI_Handler); i++) {
        *p++ = 0;
    }
    m_QSPI_Handler.Instance = QUADSPI;
    HAL_QSPI_DeInit(&m_QSPI_Handler); // ��λQSPI
    /********************/

    m_QSPI_Handler.Instance                = QUADSPI;                        // QSPIʵ��
    m_QSPI_Handler.Init.ClockPrescaler     = 1;                              // ʱ��Ԥ��Ƶ
    m_QSPI_Handler.Init.FifoThreshold      = 32;                             // FIFO��ֵ
    m_QSPI_Handler.Init.SampleShifting     = QSPI_SAMPLE_SHIFTING_HALFCYCLE; // ������λ
    m_QSPI_Handler.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_5_CYCLE;      // Ƭѡ�ߵ�ƽʱ��
    m_QSPI_Handler.Init.DualFlash          = QSPI_DUALFLASH_DISABLE;         // ��ʹ��˫����
    m_QSPI_Handler.Init.FlashID            = QSPI_FLASH_ID_1;                // ʹ�õ�һ������
    m_QSPI_Handler.Init.ClockMode          = QSPI_CLOCK_MODE_3;              // SPIģʽ3
    m_QSPI_Handler.Init.FlashSize          = POSITION_VAL(0X1000000) - 1;    // �����С
    HAL_QSPI_Init(&m_QSPI_Handler);                                          // ��ʼ��QSPI

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
    // reset��Ĭ��������QSPI
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
    cmd.NbData = 1; // ��ȡ2���ֽ�

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
    // cmd.NbData              = 2;  // ��ȡ2���ֽ�

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
    uint32_t sector_offset = Addr % 4096;          // ��ַ�������ڵ�ƫ����
    uint32_t sector_pos    = Addr / 4096;          // ��ַ��������λ��
    uint32_t sector_remain = 4096 - sector_offset; // ��ַ��������ʣ��Ĵ�С

    uint8_t buff[4096];
    while (1) {
        // ��ȡҪд������������
        if (!W25Qxx_read_len(buff, Addr - sector_offset, 4096)) {
            m_error_code = read_error;
            return 0;
        }
        if (length <= sector_remain) { // һ����������д��
            if (!W25Qxx_erase(setor_erase, Addr - sector_offset)) {
                m_error_code = erase_error;
                return 0;
            }
            // �޸�����
            for (uint32_t i = sector_offset; i < length + sector_offset; i++) {
                buff[i] = pBuff[i - sector_offset];
            }
            if (!W25Qxx_write_auto_shift_page(buff, Addr - sector_offset, 4096)) {
                m_error_code = write_error;
                return 0;
            }
            break;
        } else { // ��Ҫд����һ����
            // ����оƬ�ϵ�����
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
            // ��������
            Addr += sector_remain;
            length -= sector_remain;
            pBuff += sector_remain;
            // ���¼���
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
    // �ô���cmd.AlternateBytes ������Ϊ0x20����ʾ������ȡģʽ������һ�β���Ҫ�ٷ���0xEBָ�ֱ�Ӷ�ȡ���ݼ���
    // ���ǻᵼ����һ�η�������ָ��ʱ����һ�η��͵�ָ��Ϊ��Ч
    // ͨ����ֵΪ0����Ӱ����һ�η��͵�ָ��
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

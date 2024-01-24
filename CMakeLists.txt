CMAKE_MINIMUM_REQUIRED(VERSION 3.20)
SET(PATH_WORKSPACE_ROOT ${CMAKE_SOURCE_DIR})

#设置项目的名称
SET (RROJECT_NAME "project")
SET(PROJECT_VERSION 1.0)
#设置平台类型
SET (PALTFORM "cortex_m7")
#可通过修改下面的值爱修改输出的文件名
SET(OUTPUT_EXE_NAME ${RROJECT_NAME})

if(${PALTFORM} STREQUAL "cortex_m7")
    INCLUDE("${PATH_WORKSPACE_ROOT}/toolchain/cmake/cortex_m7.cmake")
elseif(${PALTFORM} STREQUAL "cortex_m4")
    INCLUDE("${PATH_WORKSPACE_ROOT}/toolchain/cmake/cortex_m4.cmake")
elseif(${PALTFORM} STREQUAL "cortex_m3")
    INCLUDE("${PATH_WORKSPACE_ROOT}/toolchain/cmake/cortex_m3.cmake")
elseif(${PALTFORM} STREQUAL "cortex_m0")
    INCLUDE("${PATH_WORKSPACE_ROOT}/toolchain/cmake/cortex_m0.cmake")
else()
    MESSAGE(FATAL_ERROR "PALTFORM is not supported")
endif()

# #################################################################
# 1、工程信息
# #################################################################
PROJECT(
    ${RROJECT_NAME}
    VERSION ${PROJECT_VERSION}
    LANGUAGES C CXX ASM
)

MESSAGE(STATUS "****** Building project: ${CMAKE_PROJECT_NAME} ,Version: ${CMAKE_PROJECT_VERSION}******")


if(${PALTFORM} STREQUAL "cortex_m7")
    #指定链接文件
    SET(LINKER_SCRIPT "${PATH_WORKSPACE_ROOT}/toolchain/linkscripts/stm32h750xbh6_internal_flash.ld")
    #指定启动文件
    SET(STARTUP_ASM "${PATH_WORKSPACE_ROOT}/System/STM32H7/src/startup_stm32h750xx.s")
elseif(${PALTFORM} STREQUAL "cortex_m4")
    SET(LINKER_SCRIPT "${PATH_WORKSPACE_ROOT}/toolchain/linkscripts/STM32F407ZGTx_FLASH.ld")
    SET(STARTUP_ASM "${PATH_WORKSPACE_ROOT}/System/STM32F4/src/startup_stm32f407xx.s")
elseif(${PALTFORM} STREQUAL "cortex_m3")
    SET(LINKER_SCRIPT "${PATH_WORKSPACE_ROOT}/toolchain/linkscripts/STM32F103XB_FLASH.ld")
    SET(STARTUP_ASM "${PATH_WORKSPACE_ROOT}/System/STM32F1/src/startup_stm32f103xb.s")
elseif(${PALTFORM} STREQUAL "cortex_m0")
    #SET(LINKER_SCRIPT "${PATH_WORKSPACE_ROOT}/toolchain/linkscripts/stm32h750xbh6_external_flash.ld")
    #SET(STARTUP_ASM "${PATH_WORKSPACE_ROOT}/System/startup_stm32h750xx.s")
else()
    MESSAGE(FATAL_ERROR "PALTFORM is not supported")
endif()

#项目底层公共头文件路径
INCLUDE_DIRECTORIES("${PATH_WORKSPACE_ROOT}/Driver/CMSIS/Core/Include")

# ###############################################
# 3、预定义宏；
# ###############################################

#平台相关定义
if(${PALTFORM} STREQUAL "cortex_m7")
    ADD_DEFINITIONS(-DSTM32H750xx -DHSE_VALUE=25000000)
elseif(${PALTFORM} STREQUAL "cortex_m4")
    ADD_DEFINITIONS(-DSTM32F407xx -DHSE_VALUE=8000000)
elseif(${PALTFORM} STREQUAL "cortex_m3")
    ADD_DEFINITIONS(-DSTM32F103xB -DHSE_VALUE=8000000)
elseif(${PALTFORM} STREQUAL "cortex_m0")
    ADD_DEFINITIONS(-DSTM32??-DHSE_VALUE=8000000)
else()
    MESSAGE(FATAL_ERROR "PALTFORM is not supported")
endif()
ADD_DEFINITIONS(
    -DUSE_HAL_DRIVER
    #-DUSING_NON_RTOS=0 # 不使用 RTOS
    #-DUSING_FREERTOS=1 # 使用 FreeRTOS
    #-DUSING_THREADX=2 # 使用 Threadx
    #-DUSING_RTOS=USING_NON_RTOS # 选择使用的 RTOS
)


# ##################################################
# 4、差异化构建配置；
# ##################################################

#设置一个Debug调试选项
OPTION(OPEN_LOG_OMN_DEBUG "Open log output for debug" OFF)

#优化级别的差异配置
IF("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    ADD_DEFINITIONS()
ELSEIF("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    ADD_DEFINITIONS()
ELSEIF("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    ADD_DEFINITIONS()
ELSE()
    IF(OPEN_LOG_OMN_DEBUG)
    ADD_DEFINITIONS(-DLOG_BACKEND=LOG_BACKEND_NONE)
    ELSE()
    ADD_DEFINITIONS(-DLOG_BACKEND=LOG_BACKEND_NONE)
    ENDIF()
ENDIF()

MESSAGE(STATUS "**** Build for ${CMAKE_BUILD_TYPE} ****")

# ###########################################################
# 5、设置文件输出路径；
#############################################################

#设置库的输出路径
SET(LIBRARY_OUTPUT_PATH "${CMAKE_WORKSPACE_ROOT}/build/lib_obj")

SET(ELF_FILE "${CMAKE_WORKSPACE_ROOT}/build/${OUTPUT_EXE_NAME}.elf")
SET(HEX_FILE "${CMAKE_WORKSPACE_ROOT}/build/${OUTPUT_EXE_NAME}.hex")
SET(BIN_FILE "${CMAKE_WORKSPACE_ROOT}/build/${OUTPUT_EXE_NAME}.bin")

# ###################################################################
# 6、组织公共库源文件；
# ####################################################################


# ####################################################################
# 7、组织用户源文件；
# ####################################################################

#用户源码
INCLUDE_DIRECTORIES(
    ${PATH_WORKSPACE_ROOT}/User/include
)

SET(USER_SOURCE
    ${PATH_WORKSPACE_ROOT}/User/src/main.c
)

# #####################################################################
# 8、编译、连接，生成可执行文件
# #####################################################################

# 定义连接器参数； --gc-sections：指示链接器去掉不用的 section
#Og -Wall -g --specs=nosys.specs --specs=nano.specs -u _printf_float -ffunction-sections -fdata-sections
SET(CMAKE_EXE_LINKER_FLAGS 
    "${CMAKE_EXE_LINKER_FLAGS} -T ${LINKER_SCRIPT} -Wl,-Map=${PROJECT_BINARY_DIR}/${OUTPUT_EXE_NAME}.map -Wl,--gc-sections,--print-memory-usage"
)
#生成可执行文件
ADD_EXECUTABLE(${PROJECT_NAME} ${COMMON_SERVICES_SOURCE} ${USER_SOURCE} ${LINKER_SCRIPT} ${STARTUP_ASM})
#添加依赖
if(${PALTFORM} STREQUAL "cortex_m7")
    ADD_SUBDIRECTORY(${PATH_WORKSPACE_ROOT}/Driver/STM32H7xx_HAL_Driver/ ${LIBRARY_OUTPUT_PATH}/Driver)
    ADD_SUBDIRECTORY(${PATH_WORKSPACE_ROOT}/System/STM32H7/ ${LIBRARY_OUTPUT_PATH}/System)
elseif(${PALTFORM} STREQUAL "cortex_m4")
    ADD_SUBDIRECTORY(${PATH_WORKSPACE_ROOT}/Driver/STM32F4xx_HAL_Driver/ ${LIBRARY_OUTPUT_PATH}/Driver)
    ADD_SUBDIRECTORY(${PATH_WORKSPACE_ROOT}/System/STM32F4/ ${LIBRARY_OUTPUT_PATH}/System)
elseif(${PALTFORM} STREQUAL "cortex_m3")
    ADD_SUBDIRECTORY(${PATH_WORKSPACE_ROOT}/Driver/STM32F1xx_HAL_Driver/ ${LIBRARY_OUTPUT_PATH}/Driver)
    ADD_SUBDIRECTORY(${PATH_WORKSPACE_ROOT}/System/STM32F1/ ${LIBRARY_OUTPUT_PATH}/System)
elseif(${PALTFORM} STREQUAL "cortex_m0")
    #ADD_SUBDIRECTORY(${PATH_WORKSPACE_ROOT}/Driver/STM32H7xx_HAL_Driver/ ${LIBRARY_OUTPUT_PATH}/Driver)
    #ADD_SUBDIRECTORY(${PATH_WORKSPACE_ROOT}/System/ ${LIBRARY_OUTPUT_PATH}/System)
else()
    MESSAGE(FATAL_ERROR "PALTFORM is not supported")
endif()

ADD_SUBDIRECTORY(${PATH_WORKSPACE_ROOT}/Hardware/ ${LIBRARY_OUTPUT_PATH}/Hardware)
ADD_SUBDIRECTORY(${PATH_WORKSPACE_ROOT}/Software/ ${LIBRARY_OUTPUT_PATH}/Software)
ADD_SUBDIRECTORY(${PATH_WORKSPACE_ROOT}/Utils/ ${LIBRARY_OUTPUT_PATH}/Utils)

# ###################################################################
# 9、生成 hex 和 bin 文件
# ###################################################################
ADD_CUSTOM_COMMAND(
    TARGET "${PROJECT_NAME}"
    POST_BUILD
    #生成.hex .bin
    COMMAND ${OBJCOPY} -Obinary "${PROJECT_NAME}" "${OUTPUT_EXE_NAME}.bin"
    COMMAND ${OBJCOPY} -Oihex "${PROJECT_NAME}" "${OUTPUT_EXE_NAME}.hex"
    COMMENT "Building ${OUTPUT_EXE_NAME}.bin and ${OUTPUT_EXE_NAME}.hex"
    #显示大小
    COMMAND ${SIZE} --format=berkeley ${PROJECT_NAME}
    COMMENT "Invoking:Cross ARM GNU Print Size"

    #转换为.elf
    #objcopy -I ihex input.hex -O elf32-littlearm output.elf
    COMMAND ${OBJCOPY} -I ihex "${PROJECT_NAME}" -O elf32-littlearm "${OUTPUT_EXE_NAME}.elf"
    COMMENT "hex to elf"

    #显示.elf的文件信息
    #objdump -h output.elf
    #COMMAND ${OBJDUMP} -h "${OUTPUT_EXE_NAME}.elf"
    #COMMENT "objdum result"
)
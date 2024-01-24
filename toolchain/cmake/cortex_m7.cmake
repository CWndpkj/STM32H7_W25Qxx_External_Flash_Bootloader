SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_PROCESSOR cortex-m7)

SET(THREADX_ARCH "cortex_m7")
SET(THREADX_TOOLCHAIN "gnu")

SET(MCPU_FLAGS "-mcpu=cortex-m7 -mthumb")
SET(VFP_FLAGS "-mfloat-abi=hard -mfpu=fpv5-sp-d16")

MESSAGE(STATUS "**** Platform: ${MCPU_FLAGS} ${VFP_FLAGS} ${FLOAT_ABI} ****")

INCLUDE(${CMAKE_CURRENT_LIST_DIR}/arm-none-eabi.cmake)
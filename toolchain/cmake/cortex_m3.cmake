SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_PROCESSOR cortex-m3)

SET(THREADX_ARCH "cortex_m3")
SET(THREADX_TOOLCHAIN "gnu")

SET(MCPU_FLAGS "-mcpu=cortex-m3 -mthumb")
SET(VFP_FLAGS "-mfloat-abi=soft")

MESSAGE(STATUS "**** Platform: ${MCPU_FLAGS} ${VFP_FLAGS} ${FLOAT_ABI} ****")

INCLUDE(${CMAKE_CURRENT_LIST_DIR}/arm-none-eabi.cmake)
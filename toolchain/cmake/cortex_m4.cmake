set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR cortex-m4)

set(THREADX_ARCH "cortex_m4")
set(THREADX_TOOLCHAIN "gnu")

set(MCPU_FLAGS "-mcpu=cortex-m4 -mthumb")
set(VFP_FLAGS "-mfloat-abi=hard -mfpu=fpv4-sp-d16")

message(STATUS "**** Platform: ${MCPU_FLAGS} ${VFP_FLAGS} ${FLOAT_ABI} ****")

include(${CMAKE_CURRENT_LIST_DIR}/arm-none-eabi.cmake)

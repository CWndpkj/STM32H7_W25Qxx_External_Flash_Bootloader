#编译器参数
#arm_none_eabi_toolchain的bin路径
#查找arm_none_eabi_gcc的路径
SET(TOOL_CHAIN_PREFIX "arm-none-eabi-")  
FIND_PROGRAM(ARM_NONE_EABI_GCC_EXECUTABLE NAMES ${TOOL_CHAIN_PREFIX}gcc)
if(ARM_NONE_EABI_GCC_EXECUTABLE)
    SET(CMAKE_C_COMPILER "${ARM_NONE_EABI_GCC_EXECUTABLE}")
else()
    MESSAGE(FATAL_ERROR "arm-none-eabi-gcc not found ,check whether your toolchain path has been added to PATH")
endif()
#查找arm_none_eabi_g++的路径
FIND_PROGRAM(ARM_NONE_EABI_GXX_EXECUTABLE NAMES ${TOOL_CHAIN_PREFIX}g++)
if(ARM_NONE_EABI_GXX_EXECUTABLE)
    SET(CMAKE_CXX_COMPILER "${ARM_NONE_EABI_GXX_EXECUTABLE}")
else()
    MESSAGE(WARNING "arm-none-eabi-g++ not found ,check whether your toolchain path has been added to PATH,use gcc instead")
endif()

#设置相关工具路径
get_filename_component(ARM_NONE_EABI_TOOLCHAIN_PATH ${ARM_NONE_EABI_GCC_EXECUTABLE} DIRECTORY)
#SET(ARM_NONE_EABI_TOOLCHAIN_PATH "D:/install/arm-none-eabi/bin")
#arm_none_eabi_include路径
#D:\install\arm-none-eabi\lib\gcc\arm-none-eabi\12.3.1\include
SET(ARM_NONE_EABI_INCLUDE_PATH ${ARM_NONE_EABI_TOOLCHAIN_PATH}/../lib/gcc/arm-none-eabi/12.3.1/include)
SET(AS "${ARM_NONE_EABI_TOOLCHAIN_PATH}/${TOOL_CHAIN_PREFIX}as.exe")
SET(AR "${ARM_NONE_EABI_TOOLCHAIN_PATH}/${TOOL_CHAIN_PREFIX}ar.exe")
SET(OBJCOPY "${ARM_NONE_EABI_TOOLCHAIN_PATH}/${TOOL_CHAIN_PREFIX}objcopy.exe")
SET(SIZE "${ARM_NONE_EABI_TOOLCHAIN_PATH}/${TOOL_CHAIN_PREFIX}size.exe")
SET(OBJDUMP "${ARM_NONE_EABI_TOOLCHAIN_PATH}/${TOOL_CHAIN_PREFIX}objdump.exe")

#使用的c语法
SET(CMAKE_C_STANDARD 11)
#使用的c++语法
SET(CMAKE_CXX_STANDARD 11)
#指定生成compile_commands.json
SET(CMAKE_EXPORT_COMPILE_COMMANDS True)
#彩色日志输出
SET(CMAKE_COLOR_DIAGNOSTICS True)

#设置CMake在查找可执行程序时不使用交叉编译环境的根路径。这意味着CMake将使用默认的系统路径来查找可执行程序。
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
#设置CMake在查找库文件时只使用交叉编译环境的根路径。这意味着CMake将只在交叉编译环境的根路径中查找库文件，而不会使用默认的系统路径。
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
#设置CMake在查找头文件时只使用交叉编译环境的根路径。这意味着CMake将只在交叉编译环境的根路径中查找头文件，而不会使用默认的系统路径。
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
#设置CMake在查找包时只使用交叉编译环境的根路径。这意味着CMake将只在交叉编译环境的根路径中查找包，而不会使用默认的系统路径。
SET(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

#指定生成库为静态库
# this makes the test compiles use static library option so that we don't need to pre-set linker flags and scripts
SET(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)


# 定义通用编译器参数；
# ${MCPU_FLAGS}   处理器内核信息
# ${VFP_FLAGS}    浮点运算单元类型
# ${SYSTEM_PATH}  编译器头文件路径

#设置gcc包含路径
SET(SYSTEM_PATH "-isystem ${ARM_NONE_EABI_INCLUDE_PATH}")

SET(CFCOMMON "${MCPU_FLAGS} ${VFP_FLAGS} ${SYSTEM_PATH} --specs=nano.specs  -u _printf_float  --specs=nosys.specs -Wall -fmessage-length=0 -ffunction-sections -fdata-sections")

# 定义最快运行速度发行模式的编译参数
SET(CMAKE_C_FLAGS_RELEASE "-Os ${CFCOMMON}")
SET(CMAKE_CXX_FLAGS_RELEASE "-Os -g ${CFCOMMON} -fno-exceptions")
SET(CMAKE_ASM_FLAGS_RELEASE "${MPU_FLAGS} ${VFP_FLAGS} -x assembler-with-cpp")

# 定义最小尺寸且包含调试信息的编译参数；
SET(CMAKE_C_FLAGS_RELWITHDEBINFO "-Os -g  ${CFCOMMON}")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-Os -g  ${CFCOMMON} -fno-exceptions")
SET(CMAKE_ASM_FLAGS_RELWITHDEBINFO "${MCPU_FLAGS} ${VFP_FLAGS} -x assembler-with-cpp")

# 定义最小尺寸的编译参数；
SET(CMAKE_C_FLAGS_MINSIZEREL "-Os  ${CFCOMMON}")
SET(CMAKE_CXX_FLAGS_MINSIZEREL "-Os  ${CFCOMMON} -fno-exceptions")
SET(CMAKE_ASM_FLAGS_MINSIZEREL "${MCPU_FLAGS} ${VFP_FLAGS} -x assembler-with-cpp")

# 定义调试模式编译参数；
SET(CMAKE_C_FLAGS_DEBUG "-O0 -g  ${CFCOMMON}")
SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -g  ${CFCOMMON} -fno-exceptions")
SET(CMAKE_ASM_FLAGS_DEBUG "${MCPU_FLAGS} ${VFP_FLAGS} -x assembler-with-cpp")

IF("${CMAKE_BUILD-TYPE}" STREQUAL "Release")
    MESSAGE(STATUS "****** Maximum optimization for speed ******")
ELSEIF("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    MESSAGE(STATUS "******Maximum optimization for size,debug info included******")
ELSEIF("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    MESSAGE(STATUS "******Maximum optimization for size******")
ELSE()
    MESSAGE(STATUS "******No optimization,debug info included******")
ENDIF()
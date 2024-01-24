# 要连接到构建目标的源文件；
TARGET_SOURCES(
    ${PROJECT_NAME}
    PRIVATE
    #Begin target source
    ${CMAKE_CURRENT_LIST_DIR}/system_stm32f1xx.c
    ${CMAKE_CURRENT_LIST_DIR}/sys.c
    ${CMAKE_CURRENT_LIST_DIR}/syscalls.c
    ${CMAKE_CURRENT_LIST_DIR}/stm32f1xx_it.c
    #end target sorurce
)

# 将模块头文件路径添加到目标；
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} 
    PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}/include   
)
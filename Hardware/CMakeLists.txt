# 要连接到构建目标的源文件；
TARGET_SOURCES(
    ${PROJECT_NAME}
    PRIVATE
    #Begin target source
    ${CMAKE_CURRENT_LIST_DIR}/LED/led.c
    ${CMAKE_CURRENT_LIST_DIR}/UART/uart.c
    ${CMAKE_CURRENT_LIST_DIR}/W25Qxx/w25qxx.c
    #end target sorurce
)

# 将模块头文件路径添加到目标；
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} 
    PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}/LED
    ${CMAKE_CURRENT_LIST_DIR}/UART
    ${CMAKE_CURRENT_LIST_DIR}/W25Qxx
)
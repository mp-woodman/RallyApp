# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)


add_compile_definitions(ESP32)
add_compile_definitions(ARDUINO=181)
add_compile_definitions(ARDUINO_ARCH_ESP32)
add_compile_definitions(M5STACK_MPU9250)

project(00_E_Paper_2_9_Example_VS_Code)
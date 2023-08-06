cmake_minimum_required(VERSION 3.0)
project(base64 LANGUAGES C)
set(CMAKE_C_STANDARD 99)

add_executable(${PROJECT_NAME} main.c base64.c)
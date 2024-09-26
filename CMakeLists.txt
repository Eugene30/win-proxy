cmake_minimum_required(VERSION 3.28)
project(proxy)

set(CMAKE_CXX_STANDARD 17)
set(TARGET_NAME proxy)

add_executable(proxy
    main.cpp
    resources/app.rc
)

target_compile_definitions(${TARGET_NAME} PRIVATE _UNICODE UNICODE _CONSOLE)

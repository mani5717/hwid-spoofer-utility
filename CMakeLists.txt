cmake_minimum_required(VERSION 3.10)
project(hwid-spoofer-utility VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(hwid-spoofer-utility
    src/main.cpp
    src/spoofer.cpp
    src/hwid_reader.cpp
    src/registry_utils.cpp
)

target_include_directories(hwid-spoofer-utility PRIVATE include)

if(WIN32)
    target_link_libraries(hwid-spoofer-utility PRIVATE advapi32)
endif()
cmake_minimum_required(VERSION 3.10)
project(MalwareDetector)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(MalwareDetector
    main.cpp
    scanner.cpp
    monitor.cpp
    sha256.c
)

if (WIN32)
    target_link_libraries(MalwareDetector)
else()
    message(WARNING "This project's file monitor is implemented for Windows only.")
endif()
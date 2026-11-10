cmake_minimum_required(VERSION 3.21)
project(oop_labs LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    # исходники в UTF-8, без этого строки с кириллицей ломаются
    add_compile_options(/utf-8)
endif()

find_package(Boost REQUIRED COMPONENTS serialization wserialization)

add_subdirectory(lab2)
add_subdirectory(lab6/native)

# лабы 3 и 4 на Qt собираются, только если Qt установлен
find_package(Qt6 COMPONENTS Widgets)
if(Qt6_FOUND)
    add_subdirectory(lab3)
    add_subdirectory(lab4)
else()
    message(STATUS "Qt6 не найден, lab3 и lab4 пропущены")
endif()

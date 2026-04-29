cmake_minimum_required(VERSION 3.25)
project(spscan-gui-prototype LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS
    Widgets
    Charts
)

add_executable(spscan-proto
    src/main.cpp
    src/gui/views/result_model.hpp
    src/gui/views/result_model.cpp
    src/gui/views/cvss_delegate.hpp
    src/gui/views/cvss_delegate.cpp
    src/gui/views/main_window.hpp
    src/gui/views/main_window.cpp
)

target_include_directories(spscan-proto PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/tests
)

target_link_libraries(spscan-proto PRIVATE
    Qt6::Widgets
    Qt6::Charts
)

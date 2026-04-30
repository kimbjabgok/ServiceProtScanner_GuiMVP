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
    src/gui/views/epss_delegate.hpp
    src/gui/views/epss_delegate.cpp
    src/gui/views/risk_delegate.hpp
    src/gui/views/risk_delegate.cpp
    src/gui/views/main_window.hpp
    src/gui/views/main_window.cpp
    src/gui/views/main_window.ui
    src/gui/views/details_panel.hpp
    src/gui/views/details_panel.cpp
    src/gui/views/details_panel.ui
    src/gui/views/charts_panel.hpp
    src/gui/views/charts_panel.cpp
    src/gui/views/export_actions.hpp
    src/gui/views/export_actions.cpp
    src/report/html_writer.hpp
    src/report/html_writer.cpp
    src/report/json_writer.hpp
    src/report/json_writer.cpp
    src/report/csv_writer.hpp
    src/report/csv_writer.cpp
)

target_include_directories(spscan-proto PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/tests
)

target_link_libraries(spscan-proto PRIVATE
    Qt6::Widgets
    Qt6::Charts
)

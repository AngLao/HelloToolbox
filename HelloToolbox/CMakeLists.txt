cmake_minimum_required(VERSION 3.5)

project(HelloToolbox VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS SerialPort)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS PrintSupport)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Network)

set(PROJECT_SOURCES
        main.cpp
        icon.qrc
        serialport/SerialPortBase.h
        serialport/SerialPortBase.cpp
        serialport/SerialPortBase.ui
        waveform/waveform.h
        waveform/waveform.cpp
        waveform/waveform.ui
        waveform/qcustomplot.h
        waveform/qcustomplot.cpp
        network/network.h
        network/network.cpp
        network/network.ui
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/serialport/
    ${CMAKE_CURRENT_SOURCE_DIR}/mavlink/V2.0/
    ${CMAKE_CURRENT_SOURCE_DIR}/mavlink/V2.0/common/
    ${CMAKE_CURRENT_SOURCE_DIR}/mavlink/V2.0/standard/
    ${CMAKE_CURRENT_SOURCE_DIR}/waveform/
    ${CMAKE_CURRENT_SOURCE_DIR}/network/
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(HelloToolbox
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(HelloToolbox SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(HelloToolbox
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(HelloToolbox PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::SerialPort
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::PrintSupport
    Qt${QT_VERSION_MAJOR}::Network
)

set_target_properties(HelloToolbox PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS HelloToolbox
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(HelloToolbox)
endif()



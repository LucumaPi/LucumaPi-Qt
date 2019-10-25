cmake_minimum_required(VERSION 3.5)

project(snap7-client-qt LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Network REQUIRED)

set(DISTFILES interface.qml)
set(RESOURCE img/images.qrc)
set(connecttoserver connecttoserver.h connecttoserver.cpp)

add_executable(snap7-client-qt
    ${connecttoserver}
    ${DISTFILES}
    ${RESOURCE}
    main.cpp
    mainwindow.cpp
    mainwindow.h
)
target_link_libraries(snap7-client-qt PRIVATE "/usr/local/lib/libsnap7.so")
target_link_libraries(snap7-client-qt PRIVATE Qt5::Widgets Qt5::Network)

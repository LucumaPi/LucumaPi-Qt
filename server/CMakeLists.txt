cmake_minimum_required(VERSION 3.5)

project(snap7-server-qt LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Network REQUIRED)


set(snap7
    snap7/snap7.h
    snap7/snap7.cpp)

set(connectPLC PLC/connectplc.h
    PLC/connectplc.cpp)
set(server server-architecture/server.h
    server-architecture/server.cpp
    server-architecture/plcnumber.h
    server-architecture/plcnumber.cpp
    server-architecture/dbnumber.h
    server-architecture/dbnumber.cpp
    server-architecture/serverwindow.h
    server-architecture/serverwindow.cpp)
set(RESOURCES images1.qrc)

add_executable(snap7-server-qt
  main.cpp
  mainwindow.cpp
  mainwindow.h
  ${connectPLC}
  ${snap7}
  ${server}
  ${RESOURCES}
)
target_link_libraries(snap7-server-qt PRIVATE "/usr/local/lib/libsnap7.so")
target_link_libraries(snap7-server-qt PRIVATE Qt5::Widgets Qt5::Network)

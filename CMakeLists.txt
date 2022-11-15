cmake_minimum_required(VERSION 3.14)

project(TCPclient LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets Network REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Network REQUIRED)
find_package(OpenSSL REQUIRED)

add_executable(${PROJECT_NAME}
  main.cpp
  tcpclient.cpp
  tcpclient.h
  tcpconnection.cpp
  tcpconnection.h
  iconnection.h
  tcpconnectionssl.cpp
  tcpconnectionssl.h
)

include_directories("C:/OpenSSL-Win64/include")

target_link_libraries(${PROJECT_NAME} Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network)
target_link_libraries(${PROJECT_NAME} ${OPENSSL_LIBRARIES})

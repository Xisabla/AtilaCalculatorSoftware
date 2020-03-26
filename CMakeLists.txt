cmake_minimum_required(VERSION 3.10)

include_directories(headers)
include_directories(qt)
# set the project name
project(AtilaCalculatorSoftware)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)


#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "cpp/*.cpp")

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the executable
add_executable(app 
qt/mainwindow.ui
qt/mainwindow.cpp
${SOURCES}
)

target_compile_options (app PRIVATE -std=c++17)
target_link_libraries(app Qt5::Widgets)
target_link_libraries (app -lz)
target_link_libraries (app -lgidpost)

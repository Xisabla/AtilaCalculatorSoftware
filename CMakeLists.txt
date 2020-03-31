cmake_minimum_required(VERSION 3.10)

if(POLICY CMP0020)
    cmake_policy(SET CMP0020 NEW)
    cmake_policy(SET CMP0071 NEW)
endif()

include_directories(headers)
include_directories(qt)
# set the project name
project(AtilaCalculatorSoftware)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(VTK COMPONENTS
    CommonColor
    CommonCore
    FiltersSources
    InteractionStyle
    RenderingContextOpenGL2
    RenderingCore
    RenderingFreeType
    RenderingGL2PSOpenGL2
    RenderingOpenGL2
    GUISupportQt
    QUIET
)

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
qt5_use_modules(app Core Gui)
target_link_libraries(app ${VTK_LIBRARIES})
target_compile_options (app PRIVATE -std=c++17)
target_link_libraries (app -lgidpost)
target_link_libraries (app -lz)

vtk_module_autoinit(
    TARGETS app
    MODULES ${VTK_LIBRARIES}
)

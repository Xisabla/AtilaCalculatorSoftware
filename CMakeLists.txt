cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW)
  cmake_policy(SET CMP0071 NEW)
endif()

# -----------------------------------------------------------------------------
# Project
# -----------------------------------------------------------------------------
include(cmake/Utils.cmake)

read_version()

PROJECT(AtilaCalculatorSoftware
        VERSION "${CMAKE_PROJECT_VERSION}")

message(STATUS "Building project ${PROJECT_NAME} v${PROJECT_VERSION}")

# -----------------------------------------------------------------------------
# Compiler and build config
# -----------------------------------------------------------------------------
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose Release or Debug" FORCE)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# Set code DEBUG mode if build type is Debug
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  message(STATUS "Debug mode enabled")
  add_compile_definitions(DEBUG)
endif()

# Use default cpp standard 20
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED True)
endif()

# -----------------------------------------------------------------------------
# Project files
# -----------------------------------------------------------------------------
# Qt .ui files
set(UI_FILES
        qt/MainWindow.ui)

# Include files
set(HEADERS
        include/core/binary_data.h
        include/core/binary_data_wrapper.h
        include/core/main_window.h
        include/core/mesh.h
        include/core/result.h
        include/core/utilities.h
        include/logger/common.h
        include/logger/logger.h
        include/logger/log_entries.h)

# Source files
set(SOURCES
        src/core/app.cpp
        src/core/binary_data.cpp
        src/core/binary_data_wrapper.cpp
        src/core/main_window.cpp
        src/core/mesh.cpp
        src/core/result.cpp
        src/core/utilities.cpp
        src/logger/logger.cpp
        src/logger/log_entries.cpp)

# -----------------------------------------------------------------------------
# Dependencies
# -----------------------------------------------------------------------------
# find VKT
find_package(VTK COMPONENTS
  vtkCommonColor
  vtkCommonCore
  vtkFiltersSources
  vtkInteractionStyle
  vtkInteractionWidgets
  vtkRenderingContextOpenGL2
  vtkRenderingCore
  vtkRenderingFreeType
  vtkRenderingGL2PSOpenGL2
  vtkRenderingOpenGL2
  vtkGUISupportQt
  QUIET)

# Stop if VTK is not find
if (NOT VTK_FOUND)
  message(ERROR "VTK is not installed (${VTK_NOT_FOUND_MESSAGE})")
  return ()
endif()

message(STATUS "Using VTK v${VTK_VERSION}")

# find hdf5
find_package (HDF5 COMPONENTS CXX QUIET)

if (NOT HDF5_FOUND)
  message(ERROR "HDF5 not found")
  return ()
endif()

# automoc & autouic for qt
if(vtkGUISupportQt_LOADED OR TARGET VTK::GUISupportQt)
  if (VTK_VERSION VERSION_LESS "8.90.0")
    # Instruct CMake to run moc automatically when needed.
    set(CMAKE_AUTOMOC ON)
  else()
     # Instruct CMake to run moc automatically when needed.
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTOUIC ON)
  endif()
  # We have ui files, this will bring in the macro: qt5_wrap_ui
  find_package(Qt5Widgets REQUIRED QUIET)
else()
  message(ERROR "GuiSupportQt not found.")
  return()
endif()

# Wrap UI to sources
qt5_wrap_ui(SOURCES ${UI_FILES})

# link vtk
if (VTK_VERSION VERSION_LESS "8.90.0")
  # old system
  include(${VTK_USE_FILE})
  # CMAKE_AUTOMOC in ON so the MOC headers will be automatically wrapped.
  add_executable(AtilaCalculatorSoftware MACOSX_BUNDLE
    ${SOURCES} ${HEADERS})
  qt5_use_modules(AtilaCalculatorSoftware Core Gui)
  target_link_libraries(AtilaCalculatorSoftware ${VTK_LIBRARIES})
else()
  # CMAKE_AUTOMOC in ON so the MOC headers will be automatically wrapped.
  add_executable(AtilaCalculatorSoftware MACOSX_BUNDLE
    ${SOURCES} ${HEADERS})
  qt5_use_modules(AtilaCalculatorSoftware Core Gui)
  target_link_libraries(AtilaCalculatorSoftware ${VTK_LIBRARIES})
  # vtk_module_autoinit is needed
  vtk_module_autoinit(
    TARGETS AtilaCalculatorSoftware
    MODULES ${VTK_LIBRARIES}
  )
endif()

# -----------------------------------------------------------------------------
# Build and linking instructions
# -----------------------------------------------------------------------------
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}
)

# link other libs (gidpost, zlib, hdf5)
target_link_libraries(AtilaCalculatorSoftware -lgidpost)
target_link_libraries(AtilaCalculatorSoftware -lz)
target_link_libraries(AtilaCalculatorSoftware ${HDF5_LIBRARIES})

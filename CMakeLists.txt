cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW)
  cmake_policy(SET CMP0071 NEW)
endif()

PROJECT(AtilaCalculatorSoftware)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

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

if (NOT VTK_FOUND)
  message("Skipping AtilaCalculatorSoftware: VTK is not installed (${VTK_NOT_FOUND_MESSAGE})")
  return ()
endif()

message (STATUS "VTK_VERSION: ${VTK_VERSION}")

# find hdf5
find_package (HDF5 COMPONENTS CXX QUIET)

if (NOT HDF5_FOUND)
  message("Skipping AtilaCalculatorSoftware: HDF5 not found")
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
  message(STATUS "GuiSupportQt not found.")
  return()
endif()

# source files & directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB UI_FILES src/*.ui)
file(GLOB QT_WRAP src/*.h)
file(GLOB CXX_FILES src/*.cpp)

qt5_wrap_ui(UISrcs ${UI_FILES})

# link vtk
if (VTK_VERSION VERSION_LESS "8.90.0")
  # old system
  include(${VTK_USE_FILE})
  # CMAKE_AUTOMOC in ON so the MOC headers will be automatically wrapped.
  add_executable(AtilaCalculatorSoftware MACOSX_BUNDLE
    ${CXX_FILES} ${UISrcs} ${QT_WRAP})
  qt5_use_modules(AtilaCalculatorSoftware Core Gui)
  target_link_libraries(AtilaCalculatorSoftware ${VTK_LIBRARIES})
else()
  # CMAKE_AUTOMOC in ON so the MOC headers will be automatically wrapped.
  add_executable(AtilaCalculatorSoftware MACOSX_BUNDLE
    ${CXX_FILES} ${UISrcs} ${QT_WRAP})
  qt5_use_modules(AtilaCalculatorSoftware Core Gui)
  target_link_libraries(AtilaCalculatorSoftware ${VTK_LIBRARIES})
  # vtk_module_autoinit is needed
  vtk_module_autoinit(
    TARGETS AtilaCalculatorSoftware
    MODULES ${VTK_LIBRARIES}
  )
endif()

# link other libs (gidpost, zlib, hdf5)
target_link_libraries(AtilaCalculatorSoftware -lgidpost)
target_link_libraries(AtilaCalculatorSoftware -lz)
target_link_libraries(AtilaCalculatorSoftware ${HDF5_LIBRARIES})

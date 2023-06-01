cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(avrspt-studygen)

# C++17 required
set (CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configure External Libraries
find_package(VTK COMPONENTS
  CommonCore
  ImagingGeneral
  InteractionStyle
  IOExport
  IOLegacy
  IOImage
  IOXML
  FiltersCore
  RenderingCore
  RenderingContextOpenGL2
  RenderingFreeType
  RenderingGL2PSOpenGL2
  RenderingOpenGL2
)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

if (NOT VTK_FOUND)
  message(FATAL_ERROR "Unable to find the VTK build folder.")
endif()

if (NOT ITK_FOUND)
  message(FATAL_ERROR "Unable to find the ITK build folder.")
endif()

set(LIB_EXTERNAL 
  ${ITK_LIBRARIES} 
  ${VTK_LIBRARIES}
)

set(LIB_EXTERNAL_DIRS)

# Configure Greedy
set(BUILD_CLI OFF)
set(GREEDY_BUILD_AS_SUBPROJECT TRUE)
add_subdirectory(greedy)

# Configure nlohmann_json
set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(json)

set(LIB_INTERNAL 
  util
  logic
)


set(INTERNAL_INCLUDE_DIRS 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/util
  ${CMAKE_CURRENT_SOURCE_DIR}/src/logic
)

set(EXTERNAL_INCLUDE_DIRS 
  ${CMAKE_CURRENT_SOURCE_DIR}/json/include
)

include_directories(${INTERNAL_INCLUDE_DIRS} ${EXTERNAL_INCLUDE_DIRS})

# Configure Util
add_subdirectory(src/util util)

# Configure Logic
add_subdirectory(src/logic logic)

# Core
add_subdirectory(src avrspt-studygen)

# Testing
add_subdirectory(testing)





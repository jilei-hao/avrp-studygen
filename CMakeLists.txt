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

# Download and build nlohmann_json
include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

set(LIB_EXTERNAL 
  ${ITK_LIBRARIES} 
  ${VTK_LIBRARIES}
  nlohmann_json
)

set(LIB_EXTERNAL_DIRS)

# Configure Greedy
set(BUILD_CLI OFF)
set(GREEDY_BUILD_AS_SUBPROJECT TRUE)
add_subdirectory(greedy)

# Configure nlohmann_json


set(LIB_INTERNAL 
  util
)


set(INTERNAL_INCLUDE_DIRS 
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/src/util
  ${CMAKE_CURRENT_SOURCE_DIR}/src/generator
)

set(EXTERNAL_INCLUDE_DIRS 
  ${CMAKE_CURRENT_SOURCE_DIR}/json/include
  ${CMAKE_CURRENT_SOURCE_DIR}/greedy/src
)

include_directories(${INTERNAL_INCLUDE_DIRS} ${EXTERNAL_INCLUDE_DIRS})

# Configure Util
add_subdirectory(src/util util)

# Core
add_subdirectory(src/generator generator)

# CLI
# StudyGen CLI
add_executable(StudyGen src/main.cxx)
target_link_libraries(StudyGen PRIVATE
  ${LIB_EXTERNAL}
  ${LIB_INTERNAL}
  StudyGenAPI
)

# Testing
add_subdirectory(testing)





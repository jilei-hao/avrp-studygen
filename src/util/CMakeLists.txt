message(STATUS "Configuring util...")

set (util_src 
  common.cxx
  mesh_helpers.cxx 
)

add_library(util ${util_src})
target_link_libraries(util PUBLIC ${LIB_EXTERNAL} nlohmann_json::nlohmann_json)
target_include_directories(util PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(image2mesh image_to_mesh.cxx)
target_link_libraries(image2mesh PUBLIC util)

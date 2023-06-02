message(STATUS "Configuring util...")

set (util_src 
  cl_helper.hxx
  common.cxx
  configurations.hxx
  config_factories.cxx
  format_exception.hxx
  itk_oriented_ras_image.hxx
  image_helpers.hxx 
  mesh_helpers.cxx 
)

add_library(util ${util_src})
target_link_libraries(util PRIVATE ${LIB_EXTERNAL} nlohmann_json::nlohmann_json)
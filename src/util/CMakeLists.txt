set (util_src 
  common.cxx
  configurations.hxx
  config_factories.cxx
  image_helpers.cxx 
  itk_oriented_ras_image.hxx
  mesh_helpers.cxx 
)

add_library(util ${util_src})
target_link_libraries(util PRIVATE ${LIB_EXTERNAL})

list(APPEND LIB_INTERNAL util)

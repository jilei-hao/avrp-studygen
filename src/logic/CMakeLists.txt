message(STATUS "Configuring logic...")

set (logic_src 
  mesh_processor.cxx
  study_generator_data.hxx
)

add_library(logic ${logic_src})
target_link_libraries(logic PRIVATE util ${LIB_EXTERNAL})
target_include_directories(logic PRIVATE ${INTERNAL_INCLUDE_DIRS})
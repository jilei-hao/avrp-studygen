set (logic_src 
  mesh_processor.cxx
)

add_library(logic ${logic_src})
target_link_libraries(logic PRIVATE ${LIB_INTERNAL} ${LIB_EXTERNAL})
target_include_directories(logic PRIVATE ${INTERNAL_INCLUDE_DIRS})

list(APPEND LIB_INTERNAL logic)
# StudyGenAPI 
set(studygenapi_src 
  config_factories.cxx
  study_generator.cxx
)

add_library(StudyGenAPI ${studygenapi_src})
target_include_directories(StudyGenAPI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(StudyGenAPI PUBLIC
  ${LIB_INTERNAL}
  ${LIB_EXTERNAL}
  greedyapi
  propagationapi
)
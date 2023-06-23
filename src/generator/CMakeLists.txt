# StudyGenAPI 
set(studygenapi_src 
  config_factories.cxx
  study_generator.cxx
)

add_library(StudyGenAPI ${studygenapi_src})
target_link_libraries(StudyGenAPI PRIVATE
  ${LIB_INTERNAL}
  ${LIB_EXTERNAL}
  greedyapi
  propagationapi
)
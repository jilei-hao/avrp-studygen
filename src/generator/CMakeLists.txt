# StudyGenAPI 
set(studygenapi_src 
  study_generator.cxx
  config_factories.cxx
)

set(studygenapi_include_dir
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/model_generator
  ${INTERNAL_INCLUDE_DIRS}
)

add_library(StudyGenAPI ${studygenapi_src})
target_include_directories(StudyGenAPI PUBLIC ${studygenapi_include_dir})
target_link_libraries(StudyGenAPI PUBLIC
  ${LIB_INTERNAL}
  ${LIB_EXTERNAL}
  greedyapi
  propagationapi
)
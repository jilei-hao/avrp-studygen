
set(INTERNAL_INCLUDE_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}/util
  ${CMAKE_CURRENT_SOURCE_DIR}/logic
)

message(STATUS ${INTERNAL_INCLUDE_DIRS})

# StudyGenAPI 
set(studygenapi_src 
  study_generator.cxx
)
add_library(StudyGenAPI ${studygenapi_src})
target_link_libraries(StudyGenAPI PRIVATE ${LIB_INTERNAL} ${LIB_EXTERNAL})
target_include_directories(StudyGenAPI PRIVATE ${INTERNAL_INCLUDE_DIRS})

# StudyGen CLI
set(studygen_src 
  main.cxx
)


add_executable(StudyGen ${studygen_src})
target_link_libraries(StudyGen PRIVATE ${LIB_EXTERNAL} ${LIB_INTERNAL} StudyGenAPI)
target_include_directories(StudyGen PRIVATE ${INTERNAL_INCLUDE_DIRS})
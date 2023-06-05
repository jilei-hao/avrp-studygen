add_executable(StudyGenTest test_main.cxx)
target_link_libraries(StudyGenTest PRIVATE
  ${LIB_EXTERNAL}
  ${LIB_INTERNAL}
  StudyGenAPI)
target_include_directories(StudyGenTest PRIVATE ${INTERNAL_INCLUDE_DIRS})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin )

if (CMAKE_BUILD_TYPE MATCHES DEBUG)
  message ("Debug mode")
  add_executable(main-d main.cpp)
  target_link_libraries(main-d mopsa)
else()
  message ("Release mode")
  add_executable(main main.cpp)
  target_link_libraries(main mopsa)
endif()



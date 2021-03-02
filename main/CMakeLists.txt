set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin )

add_executable(main main.cpp)
target_link_libraries(main mopsa)


cmake_minimum_required(VERSION 3.0)
project(ruba-mazzetto VERSION 0.1)

include_directories(include/)
file(GLOB_RECURSE SOURCE_FILES src/*)
set(DEVELOPMENT_FLAGS -g -Wall -Wextra)

include(options.cmake)

add_compile_options(${DEVELOPMENT_FLAGS} -Wno-unknown-pragmas)
message(${SOURCE_FILES})

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
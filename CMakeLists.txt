cmake_minimum_required(VERSION 3.12)

project(mkrandstr
        VERSION 0.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

configure_file(src/mkrandstr/version.hh.in
               src/mkrandstr/version.hh)

add_executable(${PROJECT_NAME}
               src/main.cc)

install(TARGETS ${PROJECT_NAME})


cmake_minimum_required(VERSION 3.22.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

project(grpc-server-client LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(
  gRPC
  GIT_REPOSITORY https://github.com/grpc/grpc
  GIT_TAG        v1.53.1
)
set(FETCHCONTENT_QUIET OFF)
FetchContent_MakeAvailable(gRPC)

add_library(${PROJECT_NAME} SHARED
  GrpcClientCallData.hpp
  GrpcClientCallData.impl
  GrpcServerCallData.hpp
  GrpcServerCallData.impl
  GrpcClient.hpp
  GrpcClient.impl
  GrpcServer.cpp
  GrpcServer.hpp
  GrpcServer.impl
  )

add_subdirectory(examples)
add_subdirectory(tests)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror -O0)
target_link_libraries(${PROJECT_NAME} grpc++)

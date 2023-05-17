cmake_minimum_required(VERSION 3.22.0)

project(grpc-server-client LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

include(FetchContent)
FetchContent_Declare(
  gRPC
  GIT_REPOSITORY https://github.com/grpc/grpc
  GIT_TAG        v1.53.1
)
set(FETCHCONTENT_QUIET OFF)
FetchContent_MakeAvailable(gRPC)

add_subdirectory(examples)

add_library(grpc-server-client SHARED
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

target_compile_features(grpc-server-client PRIVATE cxx_std_17)
target_compile_options(grpc-server-client PRIVATE -Wall -Wextra -Wpedantic -Werror -O0)
target_link_libraries(grpc-server-client grpc++)

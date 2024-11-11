include(proto/proto.cmake)

add_executable(grpc-server-client-tests
  Test.cpp
  "${grpc_srcs}")

target_link_libraries(grpc-server-client-tests PRIVATE grpc-server-client)

target_compile_features(grpc-server-client-tests PRIVATE cxx_std_17)

target_include_directories(grpc-server-client-tests PUBLIC ..)

find_package(GTest 1.11.0 REQUIRED)
if (GTest_FOUND)
  target_link_libraries(grpc-server-client-tests PRIVATE GTest::gtest GTest::gtest_main grpc++)
endif()

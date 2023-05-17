set(proto_srcs "${CMAKE_CURRENT_SOURCE_DIR}/SimpleService.pb.cc")
set(proto_hdrs "${CMAKE_CURRENT_SOURCE_DIR}/SimpleService.pb.h")
set(grpc_srcs "${CMAKE_CURRENT_SOURCE_DIR}/SimpleService.grpc.pb.cc")
set(grpc_hdrs "${CMAKE_CURRENT_SOURCE_DIR}/SimpleService.grpc.pb.h")
set(proto_path "${CMAKE_CURRENT_SOURCE_DIR}/proto")
set(proto "${proto_path}/SimpleService.proto")
set(protoc_command "${protobuf_BINARY_DIR}/protoc")
set(grpc_cpp_plugin "${grpc_BINARY_DIR}/grpc_cpp_plugin")
add_custom_command(
      OUTPUT "${proto_srcs}" "${proto_hdrs}" "${grpc_srcs}" "${grpc_hdrs}"
      COMMAND ${protoc_command}
      ARGS --grpc_out "${CMAKE_CURRENT_SOURCE_DIR}"
        --cpp_out "${CMAKE_CURRENT_SOURCE_DIR}"
        -I "${proto_path}"
        --plugin=protoc-gen-grpc="${grpc_cpp_plugin}"
        "${proto}")

set(common_srcs "${grpc_hdrs}" "${grpc_srcs}" "${proto_hdrs}" "${proto_srcs}")      
add_executable(simple-server "${common_srcs}" SimpleServer.cpp)
add_executable(simple-client "${common_srcs}" SimpleClient.cpp)

set(common_libs "grpc-server-client" "grpc++")
target_link_libraries(simple-server PRIVATE "${common_libs}")
target_include_directories(simple-server PUBLIC ..)
target_link_libraries(simple-client PRIVATE "${common_libs}")
target_include_directories(simple-client PUBLIC ..)



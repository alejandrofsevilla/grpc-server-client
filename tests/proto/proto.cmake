set(proto_path "${CMAKE_CURRENT_SOURCE_DIR}/proto")
set(proto_srcs "${proto_path}/SimpleService.pb.cc")
set(proto_hdrs "${proto_path}/SimpleService.pb.h")
set(grpc_srcs "${proto_path}/SimpleService.grpc.pb.cc")
set(grpc_hdrs "${proto_path}/SimpleService.grpc.pb.h")
set(proto "${proto_path}/SimpleService.proto")
set(protoc_command "${protobuf_BINARY_DIR}/protoc")
set(grpc_cpp_plugin "${grpc_BINARY_DIR}/grpc_cpp_plugin")
add_custom_command(
      OUTPUT "${proto_srcs}" "${proto_hdrs}" "${grpc_srcs}" "${grpc_hdrs}"
      COMMAND ${protoc_command}
      ARGS --grpc_out "${proto_path}"
        --cpp_out "${proto_path}"
        -I "${proto_path}"
        --plugin=protoc-gen-grpc="${grpc_cpp_plugin}"
        "${proto}")
set(grpc_srcs "${grpc_hdrs}" "${grpc_srcs}" "${proto_hdrs}" "${proto_srcs}")

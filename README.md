# [![LinuxBuildWorkflow](https://github.com/alejandrofsevilla/grpc-server-client/actions/workflows/Linux.yml/badge.svg)](https://github.com/alejandrofsevilla/grpc-server-client/actions/workflows/Linux.yml) [![TestsWorkflow](https://github.com/alejandrofsevilla/grpc-server-client/actions/workflows/LinuxBuildAndTest.yml/badge.svg)](https://github.com/alejandrofsevilla/grpc-server-client/actions/workflows/LinuxBuildAndTest.yml)
# GRPC Server and Client
C++ implementation of unary async [gRPC](https://grpc.io) Server and Client.

## Requirements
- C++17 compiler.
- CMake 3.22.0.
- gRPC 1.53.1 requirements.
- GoogleTest.

## Usage
### Simple protoservice example
```protobuf
syntax = "proto3";

service SimpleService {
  rpc SendRequest (SimpleRequest) returns (SimpleReply) {}
}

message SimpleRequest {
  string message = 1;
}

message SimpleReply {
  string message = 1;
}
```
### Server
```c++
#include <GrpcServer.hpp>
#include <iostream>

#include "SimpleService.grpc.pb.h"

GrpcServer server;
server.addListeningPort("127.0.0.1:8800", grpc::InsecureServerCredentials());
SimpleService::AsyncService service;
server.registerService(&service);
server.buildAndStart();
server.requestCallFromService<SimpleService::AsyncService, SimpleRequest,
                              SimpleReply>(
    service, &SimpleService::AsyncService::RequestSendRequest,
    [](const auto& call, [[maybe_unused]] auto& reply) {
      std::cout << call.message() << std::endl;
    });
server.startHandlingCalls();

```
### Client
```c++
#include <GrpcClient.hpp>
#include <iostream>
#include <thread>

#include "SimpleService.grpc.pb.h"

GrpcClient<SimpleService> client{grpc::CreateChannel(
    "127.0.0.1:8800", grpc::InsecureChannelCredentials())};
auto thread =
    std::thread(&GrpcClient<SimpleService>::startHandlingReplies, &client);
while (true) {
  SimpleRequest call;
  std::string message;
  std::cin >> message;
  call.set_message(message);
  client.makeCall<SimpleRequest, SimpleReply>(
      call, &SimpleService::Stub::PrepareAsyncSendRequest,
      [](const auto& reply) { std::cout << reply.message() << std::endl; });
}

```

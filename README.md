# GRPC Server and Client
Asynchronous [gRPC](https://grpc.io) Server and Client based on the [HelloWorld example](https://github.com/grpc/grpc/blob/v1.53.1/examples/cpp/helloworld), adapted to handle [unary RPCs](https://grpc.io/docs/what-is-grpc/core-concepts/#unary-rpc) from any type of Protobuf Service. <br><br>
gRPC is cloned and built as part of the project using CMake FetchContent.

## Requirements
- C++17
- CMake 3.22.0
- gRPC 1.53.1

## Example
### Protobuf Service
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

//...

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

//...
```
### Client
```c++
#include <GrpcClient.hpp>
#include <iostream>
#include <thread>

#include "SimpleService.grpc.pb.h"

//...

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

//...
```

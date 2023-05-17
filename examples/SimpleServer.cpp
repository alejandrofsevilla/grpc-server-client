#include <GrpcServer.hpp>
#include <iostream>

#include "SimpleService.grpc.pb.h"

namespace {
const std::string f_localHost{"127.0.0.1:"};
}  // namespace

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "usage: simple-server <port>" << std::endl;
    return 1;
  }
  GrpcServer server;
  server.addListeningPort(f_localHost + std::string{argv[1]},
                          grpc::InsecureServerCredentials());
  SimpleService::AsyncService service;
  server.registerService(&service);
  server.buildAndStart();
  server.requestCallFromService<SimpleService::AsyncService, SimpleRequest,
                                SimpleReply>(
      service, &SimpleService::AsyncService::RequestSendRequest,
      [](const auto& call, auto& reply) {
        std::string message{"server received: \"" + call.message() + "\""};
        std::cout << message << std::endl;
        reply.set_message(message);
      });
  server.startHandlingCalls();
  return 0;
}

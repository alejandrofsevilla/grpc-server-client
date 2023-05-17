#include <GrpcClient.hpp>
#include <iostream>
#include <thread>

#include "SimpleService.grpc.pb.h"

namespace {
std::function<void(const SimpleReply& reply)> f_replyHandler;
void doCall(GrpcClient<SimpleService>& client) {
  SimpleRequest call;
  std::string message;
  std::cout << "$";
  std::cin >> message;
  call.set_message(message);
  client.makeCall<SimpleRequest, SimpleReply>(
      call, &SimpleService::Stub::PrepareAsyncSendRequest, f_replyHandler);
}
}  // namespace

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cerr << "usage: simple-client <address> <port>" << std::endl;
    return 1;
  }
  GrpcClient<SimpleService> client{
      grpc::CreateChannel(std::string{argv[1]} + ":" + std::string{argv[2]},
                          grpc::InsecureChannelCredentials())};
  f_replyHandler = [&client](const auto& reply) {
    std::cout << reply.message() << std::endl;
    doCall(client);
  };
  auto thread =
      std::thread(&GrpcClient<SimpleService>::startHandlingReplies, &client);
  doCall(client);
  thread.join();
  return 0;
}

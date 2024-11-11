#include <chrono>
#include <gtest/gtest.h>

#include <GrpcClient.hpp>
#include <GrpcServer.hpp>
#include <random>
#include <thread>

#include "proto/SimpleService.grpc.pb.h"
#include "proto/SimpleService.pb.h"

namespace {
constexpr auto f_messageSize{500};
constexpr auto f_address{"127.0.0.1:8800"};
inline std::string generateRandomString(size_t size) {
  std::random_device rd;
  std::mt19937 eng{rd()};
  std::uniform_int_distribution<char> dist;
  std::string str(size, {});
  std::generate(str.begin(), str.end(), std::bind(dist, eng));
  return str;
}
} // namespace

TEST(GrpcTest, SimpleService) {
  GrpcServer server;
  server.addListeningPort(f_address, grpc::InsecureServerCredentials());
  SimpleService::AsyncService service;
  server.registerService(&service);
  server.buildAndStart();
  server.requestCallFromService<SimpleService::AsyncService, SimpleRequest,
                                SimpleReply>(
      service, &SimpleService::AsyncService::RequestSendRequest,
      [](const auto &call, auto &reply) { reply.set_message(call.message()); });
  std::thread t1{&GrpcServer::startHandlingCalls, &server};

  GrpcClient<SimpleService> client{
      grpc::CreateChannel(f_address, grpc::InsecureChannelCredentials())};
  std::thread t2{&GrpcClient<SimpleService>::startHandlingReplies, &client};
  SimpleRequest call;
  call.set_message(generateRandomString(f_messageSize));
  client.makeCall<SimpleRequest, SimpleReply>(
      call, &SimpleService::Stub::PrepareAsyncSendRequest,
      [&call](const auto &reply) {
        EXPECT_EQ(call.message(), reply.message());
      });

  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  server.shutDown();
  client.shutDown();

  t2.join();
  t1.join();
}

#ifndef GRPC_CLIENT_HPP
#define GRPC_CLIENT_HPP

#include <grpcpp/grpcpp.h>

#include <memory>

#include "GrpcClientCallData.hpp"

template <class Service>
class GrpcClient {
 public:
  explicit GrpcClient(std::shared_ptr<grpc::Channel> channel);

  template <class Call, class Reply>
  void makeCall(
      const Call &call,
      const typename GrpcClientCallData<typename Service::Stub, Call,
                                        Reply>::CallPreparer &callPreparer,
      const typename GrpcClientCallData<typename Service::Stub, Call,
                                        Reply>::ReplyHandler &replyHandler);

  void startHandlingReplies();

 private:
  std::unique_ptr<typename Service::Stub> m_stub;
  grpc::CompletionQueue m_completionQueue;
};

#include "GrpcClient.impl"

#endif

#ifndef GRPC_SERVER_HPP
#define GRPC_SERVER_HPP

#include <grpcpp/grpcpp.h>

#include <memory>

#include "GrpcServerCallData.hpp"

class GrpcServer {
 public:
  GrpcServer();
  ~GrpcServer();

  void addListeningPort(
      const std::string &address,
      const std::shared_ptr<grpc::ServerCredentials> &credentials);
  void registerService(grpc::Service *service);
  void buildAndStart();
  template <class Service, class Call, class Reply>
  void requestCallFromService(
      Service &service,
      const typename GrpcServerCallData<Service, Call, Reply>::CallRequest
          &request,
      const typename GrpcServerCallData<Service, Call, Reply>::CallHandler
          &handler);
  void startHandlingCalls();

 private:
  grpc::ServerBuilder m_builder;
  std::unique_ptr<grpc::ServerCompletionQueue> m_completionQueue;
  std::unique_ptr<grpc::Server> m_server;
};

#include "GrpcServer.impl"

#endif

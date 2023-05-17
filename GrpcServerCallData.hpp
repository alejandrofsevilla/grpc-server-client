#ifndef GRPC_SERVER_CALL_DATA_HPP
#define GRPC_SERVER_CALL_DATA_HPP

#include <grpcpp/grpcpp.h>

#include <functional>
#include <memory>

class GrpcServerCallDataBase {
 public:
  virtual void proceed() = 0;
};

template <class Service, class Call, class Reply>
class GrpcServerCallData final : public GrpcServerCallDataBase {
 public:
  using CallRequest = std::function<void(
      Service &, grpc::ServerContext *, Call *,
      grpc::ServerAsyncResponseWriter<Reply> *, grpc::CompletionQueue *,
      grpc::ServerCompletionQueue *, void *)>;
  using CallHandler = std::function<void(const Call &, Reply &)>;

  static void create(Service &service, const CallRequest &request,
                     const CallHandler &handler,
                     grpc::ServerCompletionQueue *completionQueue);

  void proceed();

 private:
  enum class Status { Create, Process, Finish };

  GrpcServerCallData(Service &service, const CallRequest &request,
                     const CallHandler &handler,
                     grpc::ServerCompletionQueue *completionQueue);

  const CallRequest &m_request;
  const CallHandler &m_handler;
  Service &m_service;
  Call m_call;
  Reply m_reply;
  Status m_status;
  std::unique_ptr<GrpcServerCallData> m_self;
  grpc::ServerContext m_serverContext;
  grpc::ServerAsyncResponseWriter<Reply> m_responder;
  grpc::ServerCompletionQueue *m_completionQueue;
};

#include "GrpcServerCallData.impl"

#endif

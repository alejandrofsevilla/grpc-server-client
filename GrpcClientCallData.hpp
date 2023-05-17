#ifndef GRPC_CLIENT_CALL_DATA_HPP
#define GRPC_CLIENT_CALL_DATA_HPP

#include <grpcpp/grpcpp.h>

#include <memory>

class GrpcClientCallDataBase {
 public:
  virtual void finish() = 0;
};

template <class Stub, class Call, class Reply>
class GrpcClientCallData final : public GrpcClientCallDataBase {
 public:
  using CallPreparer =
      std::function<std::unique_ptr<grpc::ClientAsyncResponseReader<Reply>>(
          Stub &, grpc::ClientContext *, const Call &,
          grpc::CompletionQueue *)>;
  using ReplyHandler = std::function<void(const Reply &)>;

  static void create(Stub &stub, const Call &call,
                     const CallPreparer &callPreparer,
                     const ReplyHandler &replyHandler,
                     grpc::CompletionQueue *completionQueue);

  void finish();

 private:
  GrpcClientCallData(Stub &stub, const Call &call,
                     const CallPreparer &callPreparer,
                     const ReplyHandler &replyHandler,
                     grpc::CompletionQueue *completionQueue);

  const ReplyHandler &m_replyHandler;

  Reply m_reply;
  std::unique_ptr<GrpcClientCallData> m_self;
  std::unique_ptr<grpc::ClientAsyncResponseReader<Reply>> m_responseReader;
  grpc::ClientContext m_clientContext;
  grpc::Status m_status;
};

#include "GrpcClientCallData.impl"

#endif

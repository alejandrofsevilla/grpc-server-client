#include "GrpcServer.hpp"

#include <iostream>

GrpcServer::GrpcServer()
    : m_builder{},
      m_completionQueue{m_builder.AddCompletionQueue()},
      m_server{} {}

GrpcServer::~GrpcServer() {
  if (m_server) {
    m_server->Shutdown();
  }
  m_completionQueue->Shutdown();
}

void GrpcServer::addListeningPort(
    const std::string &address,
    const std::shared_ptr<grpc::ServerCredentials> &credentials) {
  m_builder.AddListeningPort(address, credentials);
}

void GrpcServer::registerService(grpc::Service *service) {
  m_builder.RegisterService(service);
}

void GrpcServer::buildAndStart() { m_server = m_builder.BuildAndStart(); }

void GrpcServer::startHandlingCalls() {
  void *tag;
  bool ok;
  while (true) {
    m_completionQueue->Next(&tag, &ok);
    if (!ok) {
      std::cerr << "Error found while handling call" << std::endl;
    }
    static_cast<GrpcServerCallDataBase *>(tag)->proceed();
  }
}

//
// Created by liangzg on 25-1-8.
//

#ifndef APOLLO_UNIX_SOCKET_SERVER_H
#define APOLLO_UNIX_SOCKET_SERVER_H
#include <string>
#include <functional>
#include <atomic>
#include <memory>
#include <thread>
#include "cyber/common/log.h"
namespace apollo {
namespace dttf {
namespace util {

class UnixSocketServer {
public:
    using MessageCallback = std::function<void(const char* data, const int& len)>;
public:
    explicit UnixSocketServer(const std::string& path, const MessageCallback& cb);
    ~UnixSocketServer();

    bool Init();

    void Start();

    void Stop();

private:
    bool Bind();

    void Run() const;
private:
    std::atomic<bool> running_ = {false};
    std::unique_ptr<std::thread> server_thread_ = {nullptr};
    int server_fd_;
    std::string path_;
    MessageCallback callback_ = {nullptr};
};

} // util
} // dttf
} // apollo

#endif //APOLLO_UNIX_SOCKET_SERVER_H

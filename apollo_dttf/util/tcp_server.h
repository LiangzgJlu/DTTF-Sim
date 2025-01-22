//
// Created by liangzg on 25-1-7.
//

#ifndef TCP_SERVER_H
#define TCP_SERVER_H
#include <string>
#include <functional>
#include <atomic>
#include <memory>
#include <thread>

namespace apollo {
namespace dttf {
namespace util {


class TcpServer {
public:
    using MessageCallback = std::function<void(const char* data, const int& len)>;
public:
    explicit TcpServer(const std::string& host, const int& port, const MessageCallback& cb);
    ~TcpServer();

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
    std::string server_host_;
    int server_port_;
    MessageCallback callback_ = {nullptr};

};

} // util
} // dttf
} // apollo

#endif //TCP_SERVER_H

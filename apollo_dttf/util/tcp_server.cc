//
// Created by liangzg on 25-1-7.
//

#include "tcp_server.h"

#include <cstring>
#include <iostream>
#include <ostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include "modules/apollo_dttf/common/common.h"

namespace apollo {
namespace dttf {
namespace util {
    TcpServer::TcpServer(const std::string &host, const int &port, const MessageCallback& cb)
        : server_host_(host), server_port_(port), callback_(cb)
    {
    }

    TcpServer::~TcpServer() {
        Stop();
    }

    bool TcpServer::Init() {
        if (Bind() == false) {
            return false;
        }
        return true;
    }

    void TcpServer::Start() {
        if (running_) {
            return;
        }
        running_ = true;
        server_thread_ = std::unique_ptr<std::thread> (new std::thread(&TcpServer::Run, this));
    }

    void TcpServer::Stop() {
        if (running_) {
            running_ = false;
            if (server_thread_ && server_thread_->joinable()) {
                server_thread_->join();
            }
            if (server_fd_ > 0) {
                close(server_fd_);
            }
        }
    }

    bool TcpServer::Bind() {

        server_fd_ = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd_ < 0) {
            std::cerr << "Socket creation failed." << std::endl;
            return false;
        }
        int reuse_addr = 1;
        int ret = setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(reuse_addr));
        if (ret < 0) {
            std::cerr << "setsockopt failed: " << strerror(errno) << std::endl;
            return false;
        }

        int reuse_port = 1;
        ret = setsockopt(server_fd_, SOL_SOCKET, SO_REUSEPORT, &reuse_port, sizeof(reuse_port));
        if (ret < 0) {
            std::cerr << "setsockopt failed: " << strerror(errno) << std::endl;
            return false;
        }

        struct timeval timeout;
        timeout.tv_sec = 5;  // 设置超时时间为 5 秒
        timeout.tv_usec = 0;           // 不设置微秒部分
        ret = setsockopt(server_fd_, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
        if (ret < 0) {
            std::cerr << "setsockopt failed: " << strerror(errno) << std::endl;
            return false;
        }

        struct sockaddr_in serverAddr;
        std::memset(&serverAddr, 0, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(server_port_);
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        ret = bind(server_fd_, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
        if (ret < 0) {
            std::cerr << "bind failed: " << strerror(errno) << std::endl;
            return false;
        }
        ret = listen(server_fd_, SOMAXCONN);
        if (ret < 0) {
            std::cerr << "listen failed: " << strerror(errno) << std::endl;
            return false;
        }
        return true;
    }

    void TcpServer::Run() const {
        const int &buffer_length = 12800;
        char buffer[buffer_length];
        int bytes_received = 0;

        while (running_) {
            std::cout << "tcp " << server_host_ << ":" << server_port_ << " waiting client connection" << std::endl;
            struct sockaddr_in clientAddr;
            socklen_t clientAddrLen = sizeof(clientAddr);
            int client_fd = accept(server_fd_, (struct sockaddr*)&clientAddr, &clientAddrLen);

            if (client_fd < 0) {
                continue;
            }


            while (running_) {
                bytes_received = read(client_fd, buffer, buffer_length);
                int bytes_saved = bytes_received;

                if (bytes_received <= 0) {
                    std::cerr << "recv failed: " << strerror(errno) << std::endl;
                    break;
                }

                while (bytes_saved >= sizeof(MessageHeader)) {
                    MessageHeader header;
                    memcpy(&header, buffer, sizeof(MessageHeader));

                    if (header.magic != MAGIC_NO) {
                        break;
                    }
                    if (strcmp(header.version, VERSION) != 0) {
                        break;
                    }

                    int msg_len = 0;

                    auto msg_buffer = new char[header.msg_len];
                    // auto start = std::chrono::high_resolution_clock::now();

                    if (bytes_saved > header.msg_len) {
                        memcpy(msg_buffer, buffer + bytes_received - bytes_saved, header.msg_len);
                        msg_len = header.msg_len;

                    }else {
                        memcpy(msg_buffer, buffer + bytes_received - bytes_saved, bytes_saved);
                        msg_len += bytes_saved;
                    }
                    bytes_saved -= msg_len;
                    while (msg_len < header.msg_len && running_) {
                        bytes_received = read(client_fd, buffer, buffer_length);
                        if (bytes_received <= 0) {
                            break;
                        }
                        if (bytes_received <= header.msg_len - msg_len) {
                            memcpy(msg_buffer + msg_len, buffer, bytes_received);
                            msg_len += bytes_received;
                        }else {
                            memcpy(msg_buffer + msg_len, buffer, header.msg_len - msg_len);
                            msg_len = header.msg_len;
                            bytes_saved +=  bytes_received - (header.msg_len - msg_len);
                        }
                    }
                    if (msg_len < header.msg_len) {
                        break;
                    }
                    // std::cout << header.version << " " << header.magic << " " << header.sim_frame << " " << (std::chrono::high_resolution_clock::now().time_since_epoch() - start.time_since_epoch()).count() * 1.0 / 1e9 << std::endl;
                    if (callback_) {
                        callback_(msg_buffer, msg_len);
                    }
                    delete [] msg_buffer;
                }



            }
        }
    }
} // util
} // dttf
} // apollo
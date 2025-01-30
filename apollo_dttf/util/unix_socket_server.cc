//
// Created by liangzg on 25-1-8.
//

#include "unix_socket_server.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include "modules/apollo_dttf/common/common.h"
namespace apollo {
    namespace dttf {
        namespace util {
            UnixSocketServer::UnixSocketServer(const std::string &path, const UnixSocketServer::MessageCallback &cb)
            : path_{path}, callback_{cb}{

            }

            UnixSocketServer::~UnixSocketServer() {
                Stop();
            }

            bool UnixSocketServer::Init() {

                if(!Bind())
                {
                    return false;
                }
                return true;
            }

            void UnixSocketServer::Start() {
                if (running_) {
                    return;
                }
                running_ = true;
                server_thread_ = std::unique_ptr<std::thread> (new std::thread(&UnixSocketServer::Run, this));
            }

            void UnixSocketServer::Stop() {
                if(running_)
                {
                    running_.store(false);
                    if(server_thread_ && server_thread_->joinable())
                    {
                        server_thread_->join();
                    }

                    if(server_fd_ != -1)
                    {
                        ::close(server_fd_);
                    }
                    unlink(path_.c_str());
                }

            }

            bool UnixSocketServer::Bind() {
                server_fd_ = socket(AF_UNIX, SOCK_STREAM, 0);

                if(server_fd_ <= 0)
                {
                    return false;
                }

                unlink(path_.c_str());


                struct timeval timeout;
                timeout.tv_sec = 5;  // 设置超时时间为 5 秒
                timeout.tv_usec = 0;           // 不设置微秒部分
                int ret = setsockopt(server_fd_, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
                if (ret < 0) {
                    AWARN << "setsockopt failed: " << strerror(errno) << std::endl;
                    return false;
                }

                sockaddr_un addr;
                memset(&addr, 0, sizeof(addr));
                addr.sun_family = AF_UNIX;
                strncpy(addr.sun_path, path_.c_str(), sizeof(addr.sun_path) - 1);

                ret = bind(server_fd_, (struct sockaddr*)&addr, sizeof(addr));

                if(ret < 0){
                    AWARN << "bind error: " << strerror(errno) << std::endl;
                    
                    return false;
                }
                ret = listen(server_fd_, 1);

                if(ret < 0)
                {
                    AWARN << "listen error: " << strerror(errno) << std::endl;
                    return false;
                }

                return true;
            }

            void UnixSocketServer::Run() const {
                const int &buffer_length = 12800;
                char buffer[buffer_length];
                int bytes_received = 0;
                AINFO << "unix " << path_ << " waiting client connection" <<
                std::endl;

                while (running_) {
                    int client_fd = accept(server_fd_, nullptr, nullptr);

                    if (client_fd < 0) {
                        AINFO << "unix " << path_ << " waiting client connection" <<
                                  std::endl;

                        continue;
                    }


                    while (running_) {
                        bytes_received = read(client_fd, buffer, buffer_length);
                        int bytes_saved = bytes_received;

                        if (bytes_received <= 0) {
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
//
// Created by liangzg on 25-1-8.
//

#include "unix_socket_client.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include "cyber/cyber.h"
namespace apollo {
    namespace dttf {
        namespace util {
            UnixSocketClient::UnixSocketClient(const std::string &path)
            : path_{path}
            {

            }

            UnixSocketClient::~UnixSocketClient() {
                if(socket_ < 0)
                {
                    close(socket_);
                }
            }

            bool UnixSocketClient::Init() {
                if(!Connect())
                {
                    return false;
                }

                return true;
            }

            int UnixSocketClient::Send(const char *data, int len) {
                if (socket_ != -1) {
                    return send(socket_, data, len, MSG_NOSIGNAL);
                }
                return true;
            }

            bool UnixSocketClient::Connect() {

                socket_ = socket(AF_UNIX, SOCK_STREAM, 0);
                if (socket_ < 0) {
                    return false;
                }

                sockaddr_un addr;
                memset(&addr, 0, sizeof(addr));
                addr.sun_family = AF_UNIX;
                strncpy(addr.sun_path, path_.c_str(), path_.size());

                int ret = connect(socket_, reinterpret_cast<struct sockaddr *>(&addr), sizeof(addr));
                if (ret < 0) {
                    AWARN << "connect failed: " << strerror(errno) << std::endl;
                    return false;
                }
                return true;
            }
        } // util
    } // dttf
} // apollo

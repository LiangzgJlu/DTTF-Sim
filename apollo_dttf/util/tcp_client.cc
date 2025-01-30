//
// Created by liangzg on 25-1-7.
//

#include "tcp_client.h"

#include <cstring>
#include <iostream>
#include <ostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

namespace apollo {
namespace dttf {
namespace util {
    TcpClient::TcpClient(const std::string &host, int port)
        : host_(host), port_(port)
    {
    }

    TcpClient::~TcpClient() {
        if (socket_ != -1) {
            close(socket_);
        }
    }

    bool TcpClient::Init() {
        if (Connect() == false) {
            return false;
        }
        return true;
    }


    int TcpClient::Send(const char *data, int len) {
        if (socket_ != -1) {
            return send(socket_, data, len, MSG_NOSIGNAL);
        }
        return true;
    }

    int TcpClient::Read(char* data, int len){
        if (socket_ != -1) {
            return read(socket_, data, len);
        }
        return -1;
    }

    bool TcpClient::Connect() {

        socket_ = socket(AF_INET, SOCK_STREAM, 0);
        if (socket_ < 0) {
            return false;
        }
        int reuse_addr = 1;
        int ret = setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(reuse_addr));
        if (ret < 0) {
            std::cerr << "setsockopt failed: " << strerror(errno) << std::endl;
            return false;
        }

        int reuse_port = 1;
        ret = setsockopt(socket_, SOL_SOCKET, SO_REUSEPORT, &reuse_port, sizeof(reuse_port));
        if (ret < 0) {
            std::cerr << "setsockopt failed: " << strerror(errno) << std::endl;
            return false;
        }
        struct sockaddr_in serverAddr = {};
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port_);
        serverAddr.sin_addr.s_addr = inet_addr(host_.c_str());

        ret = connect(socket_, reinterpret_cast<struct sockaddr *>(&serverAddr), sizeof(serverAddr));
        if (ret < 0) {
            std::cerr << "connect failed: " << strerror(errno) << std::endl;
            return false;
        }
        return true;

    }
} // util
} // dttf
} // apollo
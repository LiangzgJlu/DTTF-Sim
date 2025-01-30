//
// Created by liangzg on 25-1-7.
//

#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H
#include <string>

namespace apollo {
namespace dttf {
namespace util {

class TcpClient {
public:
    explicit TcpClient(const std::string& host, int port);
    ~TcpClient();

    bool Init();

    int Send(const char* data, int len);
    int Read(char* data, int len);
private:
    bool Connect();

    int socket_ = -1;
    std::string host_;
    int port_;
};

} // util
} // dttf
} // apollo

#endif //TCP_CLIENT_H

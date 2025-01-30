//
// Created by liangzg on 25-1-8.
//

#ifndef APOLLO_UNIX_SOCKET_CLIENT_H
#define APOLLO_UNIX_SOCKET_CLIENT_H
#include <string>

namespace apollo {
namespace dttf {
namespace util {

class UnixSocketClient {
public:
    explicit UnixSocketClient(const std::string& path);
    ~UnixSocketClient();

    bool Init();

    int Send(const char* data, int len);
private:
    bool Connect();

    int socket_ = -1;
    std::string path_;
};

} // util
} // dttf
} // apollo

#endif //APOLLO_UNIX_SOCKET_CLIENT_H

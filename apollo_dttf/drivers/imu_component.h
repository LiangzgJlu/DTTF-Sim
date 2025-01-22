//
// Created by liangzg on 25-1-8.
//
#pragma once

#include <memory>
#include "modules/apollo_dttf/util/unix_socket_server.h"
#include "modules/apollo_dttf/proto/dttf_rpc.pb.h"
#include "driver_base.h"
namespace apollo {
namespace dttf {
namespace drivers {

class ImuComponent : public DriverBase {
public:
    explicit ImuComponent(const ::dttf::rpc::ImuConfig& config = {});
    ~ImuComponent() override = default;

    bool Init(const std::shared_ptr<Node>& node) override;

private:

    bool InitImu();

    void ImuDataCallback(const char* data, const int& len);

    std::unique_ptr<util::UnixSocketServer> unix_server_ {nullptr};
    std::shared_ptr<::dttf::rpc::ImuConfig> imu_config_ {nullptr};
};
} // drivers
} // dttf
} // apollo



//
// Created by liangzg on 25-1-8.
//

#pragma once

#include <memory>
#include "modules/apollo_dttf/util/unix_socket_server.h"
#include "modules/apollo_dttf/proto/dttf_rpc.pb.h"
#include "driver_base.h"
#include "modules/common_msgs/chassis_msgs/chassis.pb.h"

namespace apollo {
namespace dttf {
namespace drivers {

using apollo::canbus::Chassis;


class ChassisComponent : public DriverBase{
public:
    explicit ChassisComponent(const ::dttf::rpc::ChassisConfig& config = {});
    ~ChassisComponent() override = default;
    bool Init(const std::shared_ptr<Node>& node ) override;

private:

    bool InitChassis();

    void ChassisDataCallback(const char* data, const int& len);

    std::unique_ptr<util::UnixSocketServer> unix_server_ {nullptr};
    std::shared_ptr<::dttf::rpc::ChassisConfig> chassis_config_ {nullptr};

    std::shared_ptr<cyber::Writer<Chassis>> chassis_talker_ =
        nullptr;


    uint64_t localization_seq_num_ = 0;
};
} // drivers
} // dttf
} // apollo


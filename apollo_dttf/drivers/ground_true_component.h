//
// Created by liangzg on 25-1-8.
//

#pragma once
#include <memory>
#include <string>
#include "modules/apollo_dttf/proto/apollo_dttf.pb.h"
#include "modules/apollo_dttf/util/unix_socket_server.h"
#include "driver_base.h"
namespace apollo {
namespace dttf {
namespace drivers {

class GroundTrueComponent : public DriverBase {
public:
    explicit GroundTrueComponent(const apollo::dttf::config::GroundTrueConfig& config = {});
    ~GroundTrueComponent() override = default;
    bool Init(const std::shared_ptr<Node>& node) override;

private:

    bool InitGroundTrue();

    void GroundTrueDataCallback(const char* data, const int& len);

    std::unique_ptr<util::UnixSocketServer> unix_server_ {nullptr};
    std::shared_ptr<apollo::dttf::config::GroundTrueConfig> ground_true_config_ {nullptr};
};

} // drivers
} // dttf
} // apollo


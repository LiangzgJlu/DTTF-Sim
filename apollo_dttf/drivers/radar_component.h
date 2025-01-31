//
// Created by liangzg on 25-1-9.
//

#pragma once

#include <memory>
#include "modules/apollo_dttf/util/unix_socket_server.h"
#include "modules/apollo_dttf/proto/apollo_dttf.pb.h"
#include "driver_base.h"
namespace apollo {
namespace dttf {
namespace drivers{
class RadarComponent : public DriverBase {
public:
    explicit RadarComponent(const apollo::dttf::config::RadarConfig& config = {});
    ~RadarComponent() override = default;
    bool Init(const std::shared_ptr<Node>& node) override;

private:

    bool InitRadar();

    void RadarDataCallback(const char* data, const int& len);

    std::unique_ptr<util::UnixSocketServer> unix_server_ {nullptr};
    std::shared_ptr<apollo::dttf::config::RadarConfig> radar_config_ {nullptr};
};


}
} // dttf
} // apollo


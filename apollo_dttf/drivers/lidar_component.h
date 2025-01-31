//
// Created by liangzg on 25-1-8.
//

#pragma once

#include <memory>
#include "modules/apollo_dttf/util/unix_socket_server.h"
#include "modules/apollo_dttf/proto/apollo_dttf.pb.h"
#include "driver_base.h"
#include "modules/common_msgs/sensor_msgs/pointcloud.pb.h"
namespace apollo {
namespace dttf {
namespace drivers {

using apollo::drivers::PointCloud;
using apollo::cyber::Writer;

class LidarComponent : public DriverBase{
public:
    explicit LidarComponent(const apollo::dttf::config::LidarConfig& config = {});
    ~LidarComponent() override = default;

    bool Init(const std::shared_ptr<Node>& node) override;

private:

    bool InitLidar();

    void LidarDataCallback(const char* data, const int& len);

    std::unique_ptr<util::UnixSocketServer> unix_server_ {nullptr};
    std::shared_ptr<apollo::dttf::config::LidarConfig> lidar_config_ {nullptr};
    std::shared_ptr<Writer<PointCloud>> writer_ = nullptr;
    std::atomic<int> pcd_sequence_num_ = {0};
};

} // drivers
} // dttf
} // apollo



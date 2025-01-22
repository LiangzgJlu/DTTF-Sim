//
// Created by liangzg on 25-1-7.
//

#pragma once

#include <string>
#include <memory>
#include "driver_base.h"
#include "modules/apollo_dttf/proto/dttf_rpc.pb.h"
#include "modules/apollo_dttf/util/unix_socket_server.h"
#include "cyber/component/component.h"
#include "cyber/cyber.h"
#include "modules/common_msgs/sensor_msgs/sensor_image.pb.h"
#include "modules/transform/transform_broadcaster.h"
namespace apollo {
namespace dttf {

using apollo::cyber::Writer;
using apollo::drivers::Image;

namespace drivers {

class CameraComponent : public DriverBase{

public:
    CameraComponent(const ::dttf::rpc::CameraConfig& config = {});
    ~CameraComponent() = default;
    bool Init(const std::shared_ptr<Node>& node) override;
    
private:

    bool InitCamera();

    void ImageDataCallback(const char* data, const int& len);

    std::shared_ptr<::dttf::rpc::CameraConfig> camera_config_{nullptr};
    std::unique_ptr<util::UnixSocketServer> unix_server_ {nullptr};
    std::shared_ptr<Writer<Image>> writer_ = nullptr;
    std::unique_ptr<apollo::transform::TransformBroadcaster> tf2_broadcaster_;
};


} // driver
} // dttf
} // apollo



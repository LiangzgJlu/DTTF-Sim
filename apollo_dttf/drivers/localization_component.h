//
// Created by liangzg on 25-1-8.
//

#pragma once

#include <memory>
#include "modules/apollo_dttf/util/unix_socket_server.h"
#include "modules/apollo_dttf/proto/dttf_rpc.pb.h"
#include "driver_base.h"
#include "modules/common_msgs/localization_msgs/localization.pb.h"
#include "modules/common_msgs/localization_msgs/localization_status.pb.h"
#include "modules/transform/transform_broadcaster.h"
namespace apollo {
namespace dttf {
namespace drivers {

using apollo::localization::LocalizationEstimate;
using apollo::localization::LocalizationStatus;


class LocalizationComponent : public DriverBase{
public:
    explicit LocalizationComponent(const ::dttf::rpc::LocalizationConfig& config = {});
    ~LocalizationComponent() override = default;
    bool Init(const std::shared_ptr<Node>& node ) override;

private:

    bool InitLocalization();

    void LocalizationDataCallback(const char* data, const int& len);

    std::unique_ptr<util::UnixSocketServer> unix_server_ {nullptr};
    std::shared_ptr<::dttf::rpc::LocalizationConfig> localization_config_ {nullptr};

    std::shared_ptr<cyber::Writer<LocalizationEstimate>> localization_talker_ =
        nullptr;
    std::shared_ptr<cyber::Writer<LocalizationStatus>>
        localization_status_talker_ = nullptr;

    std::unique_ptr<apollo::transform::TransformBroadcaster> tf2_broadcaster_;

    uint64_t localization_seq_num_ = 0;
};
} // drivers
} // dttf
} // apollo


//
// Created by liangzg on 25-1-8.
//

#pragma once

#include <memory>
#include "modules/apollo_dttf/util/unix_socket_server.h"
#include "modules/apollo_dttf/proto/apollo_dttf.pb.h"
#include "driver_base.h"
#include "modules/common_msgs/localization_msgs/localization.pb.h"
#include "modules/common_msgs/localization_msgs/localization_status.pb.h"
namespace apollo {
namespace dttf {
namespace drivers {

using apollo::localization::LocalizationEstimate;
using apollo::localization::LocalizationStatus;


class GnssComponent : public DriverBase{
public:
    explicit GnssComponent(const apollo::dttf::config::GnssConfig& config = {});
    ~GnssComponent() override = default;
    bool Init(const std::shared_ptr<Node>& node ) override;

private:

    bool InitGnss();

    void GnssDataCallback(const char* data, const int& len);

    std::unique_ptr<util::UnixSocketServer> unix_server_ {nullptr};
    std::shared_ptr<apollo::dttf::config::GnssConfig> gnss_config_ {nullptr};

    std::shared_ptr<cyber::Writer<LocalizationEstimate>> localization_talker_ =
        nullptr;
    std::shared_ptr<cyber::Writer<LocalizationStatus>>
        localization_status_talker_ = nullptr;

    uint64_t localization_seq_num_ = 0;
};
} // drivers
} // dttf
} // apollo


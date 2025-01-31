#pragma once
#include <memory>
#include <unordered_map>
#include "modules/apollo_dttf/drivers/driver_base.h"
#include "cyber/component/component.h"
#include "modules/apollo_dttf/common/common.h"
#include "modules/apollo_dttf/proto/apollo_dttf.pb.h"
#include "modules/apollo_dttf/proto/autonomous_vehicle_creation_command.pb.h"
#include "modules/apollo_dttf/util/unix_socket_client.h"
#include "modules/apollo_dttf/util/tcp_client.h"
#include "modules/common_msgs/control_msgs/control_cmd.pb.h"
#include "modules/common_msgs/transform_msgs/transform.pb.h"
#include "modules/common_msgs/external_command_msgs/lane_follow_command.pb.h"
#include "modules/common_msgs/external_command_msgs/command_status.pb.h"
namespace apollo {
namespace dttf {

using apollo::control::ControlCommand;

class ApolloDttfComponent : public cyber::Component<ControlCommand> {

public:
    ApolloDttfComponent();
    ~ApolloDttfComponent();

    bool Init();

private:
    bool InitDriverComponent();

    bool SendGenerateAutonomousVehicleCommand();

    virtual bool Proc(const std::shared_ptr<ControlCommand>& msg) override;

    bool InitControl();

    bool SendTargetWaypoint();
private:
    std::unique_ptr<apollo::dttf::config::AutonomousVehicleConfig> autonomous_vehicle_config_;

    
    std::unordered_map<std::string, std::shared_ptr<drivers::DriverBase>> drivers_;
    std::unique_ptr<util::UnixSocketClient> control_unix_socket_client_;
    std::unique_ptr<util::TcpClient> dttf_command_client_;
    uint32_t control_connection_count = 0;

    std::shared_ptr<cyber::Writer<apollo::transform::TransformStampeds>> static_tf_writer_;
    apollo::transform::TransformStampeds transform_stampeds_;
    

    std::shared_ptr<
    apollo::cyber::Client<apollo::external_command::LaneFollowCommand,
                        apollo::external_command::CommandStatus>>
    lane_follow_command_client_;
};

CYBER_REGISTER_COMPONENT(ApolloDttfComponent)

}
}

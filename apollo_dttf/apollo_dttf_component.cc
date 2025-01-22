#include "apollo_dttf_component.h"

#include "modules/apollo_dttf/drivers/camera_component.h"
#include "modules/apollo_dttf/drivers/lidar_component.h"
#include "modules/apollo_dttf/drivers/gnss_component.h"
#include "modules/apollo_dttf/drivers/imu_component.h"
#include "modules/apollo_dttf/drivers/radar_component.h"
#include "modules/apollo_dttf/drivers/localization_component.h"
#include "modules/apollo_dttf/drivers/ground_true_component.h"
#include "modules/apollo_dttf/drivers/chassis_component.h"
#include "modules/apollo_dttf/common/common.h"
#include "modules/common/util/message_util.h"

// #include "proto/dttf_rpc.grpc.pb.h"
// #include <grpcpp/grpcpp.h>

namespace apollo {
namespace dttf {




ApolloDttfComponent::ApolloDttfComponent() {}

ApolloDttfComponent::~ApolloDttfComponent() {}

bool ApolloDttfComponent::Init() { 
    try
    {
        autonomous_vehicle_config_ = std::make_unique<::dttf::rpc::AutonomousVehicleConfig>();

        if(!cyber::ComponentBase::GetProtoConfig(autonomous_vehicle_config_.get()))
        {
            AERROR << "Unable to load audio conf file: "
               << ComponentBase::ConfigFilePath();
            return false;
        }

        AINFO << autonomous_vehicle_config_->ShortDebugString() << std::endl;

        if(!InitDriverComponent())
        {
            drivers_.clear();
            return false;
        }

        if(!SendGenerateAutonomousVehicleCommand())
        {
            drivers_.clear();
            return false;
        }

        if(!InitControl())
        {
            return false;
        }

        if(!SendTargetWaypoint())
        {
            return false;
        }



    }
    catch(const std::exception& e)
    {
        AERROR << e.what() << '\n';
        drivers_.clear();
        return false;
    }
    

    return true; 
    
}

bool ApolloDttfComponent::InitDriverComponent()
{
    
    
    const auto& camera_configs = autonomous_vehicle_config_->cameras();
    for(int i = 0; i < camera_configs.size(); ++i)
    {
        const auto& camera_config = camera_configs[i];
        auto camera = std::make_shared<drivers::CameraComponent>(camera_config);
        std::string name = camera_config.name();
        std::shared_ptr<drivers::DriverBase > com = std::static_pointer_cast<drivers::DriverBase >(camera);
        if(camera->Init(node_))
        {
            drivers_.insert({name, com});
            AINFO << name << " driver start.";

            
            auto transform_stamped = transform_stampeds_.add_transforms();
            transform_stamped->mutable_header()->set_frame_id("novatel");
            transform_stamped->set_child_frame_id(name);
            auto translation = transform_stamped->mutable_transform()->mutable_translation();
            translation->set_x(camera_config.location().x());
            translation->set_y(camera_config.location().y());
            translation->set_z(camera_config.location().z());
            auto rotation = transform_stamped->mutable_transform()->mutable_rotation();
            rotation->set_qx(0);
            rotation->set_qy(0);
            rotation->set_qz(0);
            rotation->set_qw(0);

        }else
        {
            AERROR << name << " driver not start.";
            return false;
        }

    }

    const auto& lidar_configs = autonomous_vehicle_config_->lidars();
    for(int i = 0; i < lidar_configs.size(); ++i)
    {
        const auto& lidar_config = lidar_configs[i];
        auto lidar = std::make_shared<drivers::LidarComponent>(lidar_config);
        std::string name = lidar_config.name();
        std::shared_ptr<drivers::DriverBase> com = std::static_pointer_cast<drivers::DriverBase >(lidar);
        if(lidar->Init(node_))
        {
            drivers_.insert({name, com});
            AINFO << name << " driver start.";
            auto transform_stamped = transform_stampeds_.add_transforms();
            transform_stamped->mutable_header()->set_frame_id("novatel");
            transform_stamped->set_child_frame_id(name);
            auto translation = transform_stamped->mutable_transform()->mutable_translation();
            translation->set_x(lidar_config.location().x());
            translation->set_y(lidar_config.location().y());
            translation->set_z(lidar_config.location().z());
            auto rotation = transform_stamped->mutable_transform()->mutable_rotation();
            rotation->set_qx(0);
            rotation->set_qy(0);
            rotation->set_qz(0);
            rotation->set_qw(0);
        }
        else
        {
            AERROR << name << " driver not start.";
            return false;
        }
    }

    const auto& radar_configs = autonomous_vehicle_config_->radars();
    for(int i = 0; i < radar_configs.size(); ++i)
    {
        const auto& radar_config = radar_configs[i];
        auto radar = std::make_shared<drivers::RadarComponent>(radar_config);
        std::string name = radar_config.name();
        std::shared_ptr<drivers::DriverBase> com = std::static_pointer_cast<drivers::DriverBase >(radar);
        if(radar->Init(node_))
        {
            drivers_.insert({name, com});
            AINFO << name << " driver start.";
        }
        else
        {
            AERROR << name << " driver not start.";
            return false;
        }
    }
    const auto& gnss_configs = autonomous_vehicle_config_->gnsses();
    for(int i = 0; i < gnss_configs.size(); ++i)
    {
        const auto& gnss_config = gnss_configs[i];
        auto gnss = std::make_shared<drivers::GnssComponent>(gnss_config);
        std::string name = gnss_config.name();
        std::shared_ptr<drivers::DriverBase> com = std::static_pointer_cast<drivers::DriverBase >(gnss);
        if(gnss->Init(node_))
        {
            drivers_.insert({name, com});
            AINFO << name << " driver start.";
        }else
        {
            AERROR << name << " driver not start.";
            return false;
        }

    }
    const auto& imu_configs = autonomous_vehicle_config_->imus();
    for(int i = 0; i < imu_configs.size(); ++i)
    {
        const auto& imu_config = imu_configs[i];
        auto imu = std::make_shared<drivers::ImuComponent>(imu_config);
        std::string name = imu_config.name();
        std::shared_ptr<drivers::DriverBase> com = std::static_pointer_cast<drivers::DriverBase >(imu);
        if(imu->Init(node_))
        {
            drivers_.insert({name, com});
            AINFO << name << " driver start.";
        }else
        {
            AERROR << name << " driver not start.";
            return false;
        }

    }

    if(autonomous_vehicle_config_->ground_true().size() > 0)
    {
        const auto& gt_config = autonomous_vehicle_config_->ground_true(0);
        auto gt = std::make_shared<drivers::GroundTrueComponent>(gt_config);
        std::string name = gt_config.name();
        std::shared_ptr<drivers::DriverBase> com = std::static_pointer_cast<drivers::DriverBase >(gt);
        if(gt->Init(node_))
        {
            drivers_.insert({name, com});
            AINFO << name << " driver start.";
        }else
        {
            AERROR << name << " driver not start.";
            return false;
        }

    }

    if(autonomous_vehicle_config_->localization_size() > 0)
    {
        const auto& localization_config = autonomous_vehicle_config_->localization(0);
        auto localization = std::make_shared<drivers::LocalizationComponent>(localization_config);
        std::string name = localization_config.name();
        std::shared_ptr<drivers::DriverBase> com = std::static_pointer_cast<drivers::DriverBase >(localization);
        if(localization->Init(node_))
        {
            drivers_.insert({name, com});
            AINFO << name << " driver start.";

            auto transform_stamped = transform_stampeds_.add_transforms();
            transform_stamped->mutable_header()->set_frame_id("localization");
            transform_stamped->set_child_frame_id("novatel");
            auto translation = transform_stamped->mutable_transform()->mutable_translation();
            translation->set_x(0);
            translation->set_y(0);
            translation->set_z(0);
            auto rotation = transform_stamped->mutable_transform()->mutable_rotation();
            rotation->set_qx(0);
            rotation->set_qy(0);
            rotation->set_qz(0);
            rotation->set_qw(0);
        }else
        {
            AERROR << name << " driver not start.";
            return false;
        }
    }

    if(autonomous_vehicle_config_->has_chassis())
    {
        const auto& chassis_config = autonomous_vehicle_config_->chassis();
        auto chassis = std::make_shared<drivers::ChassisComponent>(chassis_config);
        std::string name = chassis_config.name();
        std::shared_ptr<drivers::DriverBase> com = std::static_pointer_cast<drivers::DriverBase >(chassis);
        if(chassis->Init(node_))
        {
            drivers_.insert({name, com});
            AINFO << name << " driver start.";
        }else
        {
            AERROR << name << " driver not start.";
            return false;
        }

    }

    cyber::proto::RoleAttributes attr;
    attr.set_channel_name("/tf_static");
    attr.mutable_qos_profile()->CopyFrom(
            cyber::transport::QosProfileConf::QOS_PROFILE_TF_STATIC);
    static_tf_writer_ = node_->CreateWriter<apollo::transform::TransformStampeds>(attr);
    transform_stampeds_.mutable_header()->set_timestamp_sec(0);
    transform_stampeds_.mutable_header()->set_module_name("apollo_dttf");
    // static_tf_writer_->Write(transform_stampeds_);


    
    return true;
}

bool ApolloDttfComponent::SendGenerateAutonomousVehicleCommand()
{
    dttf_command_client_ = std::make_unique<util::TcpClient>("127.0.0.1", 50051);

    auto data = autonomous_vehicle_config_->SerializeAsString();

    if(dttf_command_client_->Init())
    {
        AutonomousVehicleConfigMessage msg;
        msg.header.magic = MAGIC_NO;
        msg.header.msg_len = sizeof(msg) + data.size();
        memcpy(&msg.header.version, VERSION, sizeof(VERSION));

        auto buffer = new char[sizeof(msg) + data.size()];

        memcpy(buffer, &msg, sizeof(msg));
        memcpy(buffer + sizeof(msg), data.c_str(), data.size());
        int ret = dttf_command_client_->Send(buffer, sizeof(msg) + data.size());
        
        delete[] buffer;

        if(ret <= 0)
        {
            return false;
        }


       
    }else{
        return false;
    }
   
    return true;
}

bool ApolloDttfComponent::Proc(const std::shared_ptr<control::ControlCommand> &msg)
{
    ControlMessage control_message;
    control_message.header.sim_time = msg->header().timestamp_sec();
    control_message.header.msg_len = sizeof(ControlMessage);
    control_message.header.magic = MAGIC_NO;
    memcpy(&control_message.header.version, VERSION, sizeof(VERSION));

    control_message.brake = msg->brake() / 100;
    control_message.throttle = msg->throttle() / 100;
    control_message.steer = -msg->steering_target() / 100;
    control_message.reverse = msg->gear_location() == ::apollo::canbus::Chassis_GearPosition::Chassis_GearPosition_GEAR_REVERSE;

    if(control_unix_socket_client_)
    {
        control_unix_socket_client_->Send(reinterpret_cast<char*>(&control_message), sizeof(ControlMessage));
    }
    return true;
}

bool ApolloDttfComponent::InitControl()
{
    try {
        const auto& control_config = autonomous_vehicle_config_->control();
        control_unix_socket_client_ = std::make_unique<util::UnixSocketClient>(control_config.communication().url());

        AINFO << "waitting connect control server... ";
        while (!control_unix_socket_client_->Init() && control_connection_count < 10)
        {
            control_connection_count ++;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        if(control_connection_count >= 10)
        {
            return false;
        }
        AINFO << "connect successfully to control server";

    }  catch (const std::exception& e) {
        AERROR << "failed to connect vehicle control server!" << e.what();
        return false;
    }
    return true;
}

bool ApolloDttfComponent::SendTargetWaypoint() { 
    
    auto lane_follow_config = autonomous_vehicle_config_->lane_following_command();

    int zone;
    double x, y;
    LatLonToUTM(autonomous_vehicle_config_->location().y(), autonomous_vehicle_config_->location().x(), x, y, zone);
    std::cout << "1: " << std::setprecision(10) << x << " " << y << std::endl;
    lane_follow_command_client_ =
      node_->CreateClient<apollo::external_command::LaneFollowCommand,
                          apollo::external_command::CommandStatus>(
          lane_follow_config.channel_name());
    
    auto lane_follow_command = std::make_shared<apollo::external_command::LaneFollowCommand>();

    lane_follow_command->set_command_id(1);

    auto way_point1 = lane_follow_command->add_way_point();
    way_point1->set_x(x);
    way_point1->set_y(y);
    way_point1->set_heading(0);

    auto way_point2 = lane_follow_command->add_way_point();
    LatLonToUTM(lane_follow_config.target_location().y(), lane_follow_config.target_location().x(), x, y, zone);
    std::cout << "2: " << std::setprecision(10) << x << " " << y << std::endl;
    way_point2->set_x(x);
    way_point2->set_y(y);
    way_point2->set_heading(lane_follow_config.target_heading());

    lane_follow_command->set_is_start_pose_set(true);

    auto end_pose = lane_follow_command->mutable_end_pose();
    end_pose->set_x(x);
    end_pose->set_y(y);
    end_pose->set_heading(lane_follow_config.target_heading());

    apollo::common::util::FillHeader("apollo_dttf", lane_follow_command.get());
    lane_follow_command_client_->SendRequest(lane_follow_command);
    return true; 
}

}  // namespace dttf
}

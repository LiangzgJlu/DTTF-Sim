//
// Created by liangzg on 25-1-8.
//

#include "chassis_component.h"

#include "modules/apollo_dttf/common/common.h"
namespace apollo {
namespace dttf {
namespace drivers {
    ChassisComponent::ChassisComponent(const ::dttf::rpc::ChassisConfig& config) {
        chassis_config_ = std::make_shared<::dttf::rpc::ChassisConfig>(config);
    }

    bool ChassisComponent::Init(const std::shared_ptr<Node>& node) {

        try{
            if(!InitChassis())
            {
              return false;
            }

            chassis_talker_ = node->CreateWriter<Chassis>
                (chassis_config_->channel_name());

        }catch (const std::exception& e)
        {
            AINFO << e.what();
            return false;
        }

        return true;
    }

    bool ChassisComponent::InitChassis()
    {

        unix_server_ = std::unique_ptr<util::UnixSocketServer>(new util::UnixSocketServer(
                chassis_config_->communication().url(),
                std::bind(&ChassisComponent::ChassisDataCallback, this,
                          std::placeholders::_1, std::placeholders::_2)));
        if (!unix_server_->Init()) {
            return false;
        }
        unix_server_->Start();
        return true;
    }

    void ChassisComponent::ChassisDataCallback(const char* data, const int& len)
    {
        if (len > 0) {
            ChassisMessage* chassis_message = (ChassisMessage*)data;
//            AINFO << "chassis message arrived!";

            auto chassis = std::make_shared<Chassis>();
            chassis->mutable_header()->set_timestamp_sec(chassis_message->header.sim_time);
            chassis->mutable_header()->set_sequence_num(++localization_seq_num_);
            chassis->mutable_header()->set_module_name("apollo_dttf");
            chassis->mutable_header()->set_frame_id("chassis");
            chassis->set_driving_mode(apollo::canbus::Chassis_DrivingMode::Chassis_DrivingMode_COMPLETE_AUTO_DRIVE);
            chassis->set_engine_started(true);
            chassis->set_engine_rpm(chassis_message->engine_rpm);
            chassis->set_speed_mps(chassis_message->speed);
            chassis->set_odometer_m(chassis_message->odometer_m);
            chassis->set_fuel_range_m(chassis_message->fuel_range_m);
            chassis->set_throttle_percentage(chassis_message->throttle_percentage * 100);
            chassis->set_brake_percentage(chassis_message->brake_percentage * 100);
            chassis->set_steering_percentage(-chassis_message->steering_percentage * 100);
            chassis->set_parking_brake(false);
            chassis->set_gear_location(static_cast<::apollo::canbus::Chassis_GearPosition>(chassis_message->gear_location));
            chassis->mutable_vehicle_id()->set_vin(chassis_config_->name());
            chassis->mutable_vehicle_id()->set_other_unique_id("ch");
            chassis->set_throttle_percentage_cmd(chassis_message->throttle_percentage_cmd * 100);
            chassis->set_brake_percentage_cmd(chassis_message->brake_percentage_cmd * 100);
            chassis->set_steering_percentage_cmd(-chassis_message->steering_percentage_cmd * 100);
            chassis_talker_->Write(chassis);
        }
    }

} // drivers
} // dttf
} // apollo

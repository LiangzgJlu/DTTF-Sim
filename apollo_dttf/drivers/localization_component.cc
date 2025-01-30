//
// Created by liangzg on 25-1-8.
//

#include "localization_component.h"
#include "cyber/time/clock.h"
#include "modules/apollo_dttf/common/common.h"
namespace apollo {
namespace dttf {
namespace drivers {
    LocalizationComponent::LocalizationComponent(const ::dttf::rpc::LocalizationConfig& config) {
        localization_config_ = std::make_shared<::dttf::rpc::LocalizationConfig>(config);
    }

    bool LocalizationComponent::Init(const std::shared_ptr<Node>& node) {

        try{
            if(!InitLocalization())
            {
              return false;
            }

            localization_talker_ = node->CreateWriter<LocalizationEstimate>
                (localization_config_->channel_name());
            localization_status_talker_ = node->CreateWriter<LocalizationStatus>(localization_config_->status_channel_name());

            tf2_broadcaster_.reset(new apollo::transform::TransformBroadcaster(node));

        }catch (const std::exception& e)
        {
            AINFO << e.what();
            return false;
        }

        return true;
    }

    bool LocalizationComponent::InitLocalization()
    {

        unix_server_ = std::unique_ptr<util::UnixSocketServer>(new util::UnixSocketServer(
                localization_config_->communication().url(),
                std::bind(&LocalizationComponent::LocalizationDataCallback, this,
                          std::placeholders::_1, std::placeholders::_2)));
        // cyber::Clock::SetMode(apollo::cyber::proto::MODE_MOCK);
        if (!unix_server_->Init()) {
            return false;
        }
        unix_server_->Start();
        return true;
    }

    void LocalizationComponent::LocalizationDataCallback(const char* data, const int& len)
    {
        if (len > 0) {
            LocalizationMessage* localization_message = (LocalizationMessage*)data;
//            AINFO << "localization message arrived!";
            auto localization = std::make_shared<LocalizationEstimate>();
            localization->mutable_header()->set_timestamp_sec(localization_message->header.sim_time);
            localization->mutable_header()->set_sequence_num(++localization_seq_num_);
            localization->mutable_header()->set_module_name("apollo_dttf");
            localization->mutable_header()->set_frame_id("localization");
            localization->set_measurement_time(localization_message->header.sim_time);


            int zone;
            double x, y;
            LatLonToUTM(localization_message->location.y, localization_message->location.x, x, y, zone);


            auto pose = localization->mutable_pose();
            pose->mutable_position()->set_x(x);
            pose->mutable_position()->set_y(y); 
            // pose->mutable_position()->set_z(localization_message->location.z);
            

            // double apollo_yaw, apollo_roll, apollo_pitch;
            // carlaToApolloEuler(localization_message->rotation.yaw / 180 * M_PI, localization_message->rotation.pitch / 180 * M_PI, localization_message->rotation.roll / 180 * M_PI, apollo_yaw, apollo_pitch, apollo_roll);

       

            Quaternion q = eulerToQuaternion(localization_message->rotation.yaw / 180 * M_PI, localization_message->rotation.roll / 180 * M_PI, localization_message->rotation.pitch / 180 * M_PI);


            pose->mutable_orientation()->set_qx(q.x);
            pose->mutable_orientation()->set_qy(q.y);
            pose->mutable_orientation()->set_qz(q.z);
            pose->mutable_orientation()->set_qw(q.w);

            pose->mutable_linear_velocity()->set_x(-localization_message->speed.y);
            pose->mutable_linear_velocity()->set_y(localization_message->speed.x);
            pose->mutable_linear_velocity()->set_z(localization_message->speed.z);

            pose->mutable_linear_acceleration()->set_x(-localization_message->acceleration.y);
            pose->mutable_linear_acceleration()->set_y(localization_message->acceleration.x);
            pose->mutable_linear_acceleration()->set_z(localization_message->acceleration.z);

            pose->mutable_angular_velocity()->set_x(localization_message->angular_velocity.y);
            pose->mutable_angular_velocity()->set_y(localization_message->angular_velocity.x);
            pose->mutable_angular_velocity()->set_z(localization_message->angular_velocity.z);

            // pose->set_heading(localization_message->heading + M_PI_2);
            pose->set_heading(localization_message->heading);

            pose->mutable_euler_angles()->set_x(localization_message->rotation.pitch / 180 * M_PI);
            pose->mutable_euler_angles()->set_y(localization_message->rotation.roll / 180 * M_PI);
            pose->mutable_euler_angles()->set_z(localization_message->rotation.yaw / 180 * M_PI);
            
            localization_talker_->Write(localization);

            auto localization_status = std::make_shared<LocalizationStatus>();

            localization_status->mutable_header()->set_timestamp_sec(localization_message->header.sim_time);
            localization_status->mutable_header()->set_sequence_num(++localization_seq_num_);
            localization_status->mutable_header()->set_module_name("apollo_dttf");
            localization_status->mutable_header()->set_frame_id("localization");
            localization_status->set_measurement_time(localization_message->header.sim_time);
            localization_status->set_fusion_status(apollo::localization::MeasureState::OK);
            localization_status->set_state_message("");
            localization_status_talker_->Write(localization_status);


            
            // cyber::Clock::SetNowInSeconds(localization->header().timestamp_sec());

            apollo::transform::TransformStamped tf2_msg;

            auto mutable_head = tf2_msg.mutable_header();
            mutable_head->set_timestamp_sec(localization->measurement_time());
            mutable_head->set_frame_id("world");
            tf2_msg.set_child_frame_id("localization");

            auto mutable_translation = tf2_msg.mutable_transform()->mutable_translation();
            mutable_translation->set_x(localization->pose().position().x());
            mutable_translation->set_y(localization->pose().position().y());
            mutable_translation->set_z(localization->pose().position().z());

            auto mutable_rotation = tf2_msg.mutable_transform()->mutable_rotation();
            mutable_rotation->set_qx(localization->pose().orientation().qx());
            mutable_rotation->set_qy(localization->pose().orientation().qy());
            mutable_rotation->set_qz(localization->pose().orientation().qz());
            mutable_rotation->set_qw(localization->pose().orientation().qw());
            tf2_broadcaster_->SendTransform(tf2_msg);
        }
    }

} // drivers
} // dttf
} // apollo

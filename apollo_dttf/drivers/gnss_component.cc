//
// Created by liangzg on 25-1-8.
//

#include "gnss_component.h"

#include "modules/apollo_dttf/common/common.h"
namespace apollo {
namespace dttf {
namespace drivers {
    GnssComponent::GnssComponent(const ::dttf::rpc::GnssConfig& config) {
        gnss_config_ = std::make_shared<::dttf::rpc::GnssConfig>(config);
    }

    bool GnssComponent::Init(const std::shared_ptr<Node>& node) {

        try{
            if(!InitGnss())
            {
              return false;
            }

            localization_talker_ = node->CreateWriter<LocalizationEstimate>
                (gnss_config_->channel_name());
            localization_status_talker_ = node->CreateWriter<LocalizationStatus>(gnss_config_->status_channel_name());

        }catch (const std::exception& e)
        {
            AINFO << e.what();
            return false;
        }

        return true;
    }

    bool GnssComponent::InitGnss()
    {

        unix_server_ = std::unique_ptr<util::UnixSocketServer>(new util::UnixSocketServer(
                gnss_config_->communication().url(),
                std::bind(&GnssComponent::GnssDataCallback, this,
                          std::placeholders::_1, std::placeholders::_2)));
        if (!unix_server_->Init()) {
            return false;
        }
        unix_server_->Start();
        return true;
    }

    void GnssComponent::GnssDataCallback(const char* data, const int& len)
    {
        // if (len > 0) {
        //     GnssMessage* gnss_message = (GnssMessage*)data;
        //     auto localization = std::make_shared<LocalizationEstimate>();
        //     localization->mutable_header()->set_timestamp_sec(gnss_message->header.sim_time);
        //     localization->mutable_header()->set_sequence_num(++localization_seq_num_);
        //     localization->mutable_header()->set_frame_id();
        //     localization->mutable_header()->set_module_name("apollo_dttf");
        //     localization->mutable_header()->set_frame_id("localization");
        //     localization->set_measurement_time(gnss_message->sim_time);
        //     auto pos = localization->mutable_pose()->mutable_position();
        //     pos->set_x(gnss_message->x);
        //     pos->set_y(gnss_message->y);
        //     pos->set_z(gnss_message->z);
        //     localization->mutable_pose()->set_heading(gnss_message->);
        // }


    }

} // drivers
} // dttf
} // apollo
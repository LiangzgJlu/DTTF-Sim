//
// Created by liangzg on 25-1-9.
//

#include "radar_component.h"

#include "modules/apollo_dttf/common/common.h"
#include <iostream>
namespace apollo {
    namespace dttf {
        namespace drivers
        {
            RadarComponent::RadarComponent(const apollo::dttf::config::RadarConfig& config) {
                radar_config_ = std::make_shared<apollo::dttf::config::RadarConfig>(config);
                
            }

            bool RadarComponent::Init(const std::shared_ptr<Node>& node) {
                if(!InitRadar())
                {
                    return false;
                }
                return true;
            }

            bool RadarComponent::InitRadar() {
                try
                {
                    unix_server_ = std::unique_ptr<util::UnixSocketServer>(new util::UnixSocketServer(
                        radar_config_->communication().url(),
                        std::bind(&RadarComponent::RadarDataCallback, this,
                                  std::placeholders::_1, std::placeholders::_2)));
                    if (!unix_server_->Init()) {
                        return false;
                    }
                    unix_server_->Start();
                }
                catch(const std::exception& e)
                {
                    AERROR << e.what() << std::endl;
                    return false;
                }

                return true;
            }

            void RadarComponent::RadarDataCallback(const char *data, const int &len) {
                if(len)
                {
                    RadarMessage* radar_message = (RadarMessage*)data;
                    AINFO << "radar " << radar_message->header.sim_frame << " " << radar_message->header.sim_time <<
                              std::endl;

                }
            }
        }



    } // dttf
} // apollo
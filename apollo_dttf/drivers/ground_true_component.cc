//
// Created by liangzg on 25-1-8.
//

#include "ground_true_component.h"

#include "modules/apollo_dttf/common/common.h"
#include <iostream>
namespace apollo {
    namespace dttf {
        namespace drivers {
            GroundTrueComponent::GroundTrueComponent(const ::dttf::rpc::GroundTrueConfig& config) {
                ground_true_config_ = std::make_shared<::dttf::rpc::GroundTrueConfig>(config);
                
            }

            bool GroundTrueComponent::Init(const std::shared_ptr<Node>& node) {
                if(!InitGroundTrue())
                {
                    return false;
                }
                return true;
            }

            bool GroundTrueComponent::InitGroundTrue() {

                try
                {
                    unix_server_ = std::unique_ptr<util::UnixSocketServer>(new util::UnixSocketServer(
                        ground_true_config_->communication().url(),
                        std::bind(&GroundTrueComponent::GroundTrueDataCallback, this,
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

            void GroundTrueComponent::GroundTrueDataCallback(const char *data, const int &len) {

                if(len > 0)
                {
                    GroundTrueObjectMessage* ground_true_message = (GroundTrueObjectMessage*)data;
                    AINFO << "ground ture: " << ground_true_message->header.sim_frame << " " <<
                    ground_true_message->header.sim_time << std::endl;

                    ObjectMessage* object_list = (ObjectMessage*)(ground_true_message + 1);

                    if(ground_true_message->object_count > 0)
                    {
                        std::cout << "object: " << object_list[0].object_type << std::endl;
                    }
                }

            }
        } // drivers
    } // dttf
} // apollo

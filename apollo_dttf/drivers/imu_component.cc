//
// Created by liangzg on 25-1-8.
//

#include "imu_component.h"

#include "modules/apollo_dttf/common/common.h"

namespace apollo {
namespace dttf {
namespace drivers {



    ImuComponent::ImuComponent(const apollo::dttf::config::ImuConfig& config) {
        imu_config_ = std::make_shared<apollo::dttf::config::ImuConfig>(config);
        
    }

    bool ImuComponent::Init(const std::shared_ptr<Node>& node) {

        if(!InitImu())
        {
            return false;
        }


        return true;
    }

    bool ImuComponent::InitImu()
    {
        try
        {
            unix_server_ = std::unique_ptr<util::UnixSocketServer>(new util::UnixSocketServer(
                    imu_config_->communication().url(),
                    std::bind(&ImuComponent::ImuDataCallback, this,
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

    void ImuComponent::ImuDataCallback(const char* data, const int& len)
    {
        if (len > 0) {
            ImuMessage* imu_message = (ImuMessage*)data;
            std::cout << "imu: " << imu_message->header.sim_frame << " " << imu_message->header.sim_time << " " <<
            imu_message->heading << std::endl;

        }
    }

} // drivers
} // dttf
} // apollo
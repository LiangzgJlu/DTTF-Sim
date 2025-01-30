//
// Created by liangzg on 25-1-8.
//

#include "lidar_component.h"

#include "modules/apollo_dttf/common/common.h"
// #include <pcl/point_types.h>
//#include <pcl/visualization/pcl_visualizer.h>
// #include <pcl/io/pcd_io.h>
namespace apollo {
namespace dttf {
namespace drivers {

//    std::shared_ptr<pcl::visualization::PCLVisualizer> viewer;


    LidarComponent::LidarComponent(const ::dttf::rpc::LidarConfig& config) {
        lidar_config_ = std::make_shared<::dttf::rpc::LidarConfig>(config);
    }

    bool LidarComponent::Init(const std::shared_ptr<Node>& node) {

        try
        {
            writer_ = node->CreateWriter<PointCloud>(lidar_config_->channel_name());

            if(!InitLidar())
            {
                return false;
            }
        }
        catch(const std::exception& e)
        {
            AERROR << e.what() << '\n';
            return false;
        }
    
        return true;
    }

    bool LidarComponent::InitLidar()
    {
        unix_server_ = std::unique_ptr<util::UnixSocketServer>(new util::UnixSocketServer(
                lidar_config_->communication().url(),
                std::bind(&LidarComponent::LidarDataCallback, this,
                          std::placeholders::_1, std::placeholders::_2)));
        if (!unix_server_->Init()) {
            return false;
        }
        unix_server_->Start();

        return true;
    }

    void LidarComponent::LidarDataCallback(const char* data, const int& len)
    {
        if (len > 0) {

            LidarMessage* lidar_message = (LidarMessage*)data;
            // AINFO << "lidar: " << lidar_message->header.sim_frame << " " << lidar_message->horizontal_angle << " " <<
            // lidar_message->channels << " " << lidar_message->point_count << " " << sizeof(LidarMessage)
            // << std::endl;

            const float* point_array = (float*)(lidar_message + 1);

            auto cloud = std::shared_ptr<PointCloud>(new PointCloud);

            cloud->mutable_header()->set_frame_id(lidar_config_->name());
            cloud->mutable_header()->set_sequence_num(pcd_sequence_num_.fetch_add(1));
            cloud->mutable_header()->set_timestamp_sec(lidar_message->header.sim_time);
            cloud->set_measurement_time(lidar_message->header.sim_time);

            cloud->set_frame_id(lidar_config_->name());
            cloud->set_is_dense(false);
            cloud->set_width(lidar_message->point_count);
            cloud->set_height(1);

            for(int i = 0; i < lidar_message->point_count; ++i)
            {
                auto point = cloud->add_point();
                point->set_x(point_array[i * 4 + 0]);
                point->set_y(-point_array[i * 4 + 1]);
                point->set_z(point_array[i * 4 + 2]);
                point->set_intensity(point_array[i * 4 + 3] * 255);
                // point->set_timestamp(static_cast<uint64_t>(lidar_message->header.sim_time * 1e9));
            }

            writer_->Write(cloud);
        
        }
    }

} // drivers
} // dttf
} // apollo

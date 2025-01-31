//
// Created by liangzg on 25-1-7.
//

#include "camera_component.h"

// #include <opencv2/highgui.hpp>
// #include <opencv2/imgproc.hpp>
// #include <opencv2/core/mat.hpp>

#include "modules/apollo_dttf/common/common.h"
#include "modules/apollo_dttf/util/unix_socket_server.h"
namespace apollo {
namespace dttf {
namespace drivers {


    CameraComponent::CameraComponent(const apollo::dttf::config::CameraConfig& config) {
        camera_config_ = std::make_shared<apollo::dttf::config::CameraConfig>(config);
    }

    bool CameraComponent::Init(const std::shared_ptr<Node>& node) {
        try
        {
            if (!InitCamera()) {
                AINFO << "Failed to init camera" << std::endl;
                return false;
            }

            writer_ = node->CreateWriter<Image>(camera_config_->channel_name());
            tf2_broadcaster_.reset(new apollo::transform::TransformBroadcaster(node));

        }
        catch(const std::exception& e)
        {
            AERROR << e.what() << '\n';
            return false;
        }
      

      return true;
    }

    bool CameraComponent::InitCamera() {

        try
        {
            unix_server_ = std::unique_ptr<util::UnixSocketServer>(new util::UnixSocketServer(
                                                        camera_config_->communication().url(),
                                                        std::bind(&CameraComponent::ImageDataCallback, this,
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

    void CameraComponent::ImageDataCallback(const char* data, const int& len) {
        if (len > 0) {
            CameraMessage* camera_message = (CameraMessage*)data;
            // AINFO << "image: " << camera_message->header.sim_frame << " " << camera_message->width << " " <<
            // camera_message->height << std::endl;
            auto image = std::shared_ptr<Image>(new Image);
            image->set_width(camera_message->width);
            image->set_height(camera_message->height);
            image->set_encoding("rgb8");
            image->set_step(3 * camera_message->width);
            image->mutable_header()->set_frame_id(camera_config_->name());
            
            image->mutable_header()->set_timestamp_sec(camera_message->header.sim_time);
            image->set_measurement_time(camera_message->header.sim_time);
            image->set_data(data + sizeof(CameraMessage), camera_message->length);
            writer_->Write(image);

            apollo::transform::TransformStamped tf2_msg;
            auto mutable_head = tf2_msg.mutable_header();
            mutable_head->set_timestamp_sec(image->measurement_time());
            mutable_head->set_frame_id("novatel");
            tf2_msg.set_child_frame_id(camera_config_->name());

            auto mutable_translation = tf2_msg.mutable_transform()->mutable_translation();
            mutable_translation->set_x(camera_config_->location().x());
            mutable_translation->set_y(camera_config_->location().y());
            mutable_translation->set_z(camera_config_->location().z());

            auto mutable_rotation = tf2_msg.mutable_transform()->mutable_rotation();
            mutable_rotation->set_qx(0);
            mutable_rotation->set_qy(0);
            mutable_rotation->set_qz(0);
            mutable_rotation->set_qw(0);
            // tf2_broadcaster_->SendTransform(tf2_msg);
        }
    }
} // driver
} // dttf
} // apollo
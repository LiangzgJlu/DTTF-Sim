load("//tools:cpplint.bzl", "cpplint")
load("//tools:apollo_package.bzl", "apollo_package", "apollo_cc_library", "apollo_component", "apollo_cc_test")

package(default_visibility = ["//visibility:public"])

apollo_cc_library(
    name = "apollo_dttf_drivers",
        srcs = [
        "camera_component.cc",
        "gnss_component.cc",
        "lidar_component.cc",
        "imu_component.cc",
        "ground_true_component.cc",
        "radar_component.cc",
        "localization_component.cc",
        "chassis_component.cc",
    ],
    hdrs = [
        "driver_base.h",
        "camera_component.h",
        "gnss_component.h",
        "lidar_component.h",
        "imu_component.h",
        "ground_true_component.h",
        "radar_component.h",
        "localization_component.h",
        "chassis_component.h",


    ],
    deps = [
        "//cyber",
        "//modules/common/math",
        "//modules/common/monitor_log",
        "//modules/common_msgs/basic_msgs:geometry_cc_proto",
        "//modules/common/util:util_tool",
        "//modules/common/util:message_util",
        "//modules/common_msgs/transform_msgs:transform_cc_proto",
        "//modules/common_msgs/localization_msgs:localization_cc_proto",
        "//modules/transform:apollo_transform",
        "//modules/common_msgs/basic_msgs:error_code_cc_proto",
        "//modules/common_msgs/basic_msgs:header_cc_proto",
        "//modules/common_msgs/sensor_msgs:sensor_image_cc_proto",
        "//modules/common_msgs/sensor_msgs:pointcloud_cc_proto",
        "//modules/common_msgs/chassis_msgs:chassis_cc_proto",
        "//modules/common_msgs/control_msgs:control_cmd_cc_proto",
        "//modules/common_msgs/external_command_msgs:chassis_command_cc_proto",
        "//modules/apollo_dttf/proto:dttf_rpc_proto",
        "//modules/apollo_dttf/util:apollo_dttf_util",
        "//modules/apollo_dttf/common:apollo_dttf_common",
        "@eigen",
       
    ],
)



apollo_package()
cpplint()

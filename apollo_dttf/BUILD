load("//tools:apollo_package.bzl", "apollo_cc_library", "apollo_cc_test", "apollo_component", "apollo_package")
load("//tools:cpplint.bzl", "cpplint")
load("//third_party/gpus:common.bzl", "gpu_library", "if_cuda", "if_rocm")

package(default_visibility = ["//visibility:public"])

apollo_component(
    name = "libapollo_dttf_component.so",
    srcs = [
        "apollo_dttf_component.cc",
        
    ],
    hdrs = [
        "apollo_dttf_component.h",
       

    ],
    linkopts = ["-fopenmp"],
    deps = [
        "//cyber",
        "//modules/common/math",
        "//modules/common/monitor_log",
        "//modules/common_msgs/basic_msgs:geometry_cc_proto",
        "//modules/common/util:util_tool",
        "//modules/common_msgs/transform_msgs:transform_cc_proto",
        "//modules/common_msgs/localization_msgs:localization_cc_proto",
        "//modules/transform:apollo_transform",
        "//modules/common_msgs/external_command_msgs:command_status_cc_proto",
        "//modules/common_msgs/external_command_msgs:lane_follow_command_cc_proto",
        "//modules/common_msgs/basic_msgs:error_code_cc_proto",
        "//modules/common_msgs/basic_msgs:header_cc_proto",
        "//modules/common_msgs/sensor_msgs:sensor_image_cc_proto",
        "//modules/common_msgs/sensor_msgs:pointcloud_cc_proto",
        "//modules/apollo_dttf/proto:apollo_dttf_proto",
        "//modules/apollo_dttf/drivers:apollo_dttf_drivers",
        "//modules/common_msgs/transform_msgs:transform_cc_proto",
        "//third_party/tf2",
        "//modules/common/adapters:adapter_gflags",
        "@eigen",
    ],
)


filegroup(
    name = "runtime_data",
    srcs = glob([
        "conf/**",
        "dag/*.dag",
        "launch/*.launch",
    ]),
)

apollo_package()

cpplint()

load("//tools:cpplint.bzl", "cpplint")
load("//tools:apollo_package.bzl", "apollo_package", "apollo_cc_library", "apollo_component", "apollo_cc_test")

package(default_visibility = ["//visibility:public"])

apollo_cc_library(
    name = "apollo_dttf_util",
        srcs = [
            "unix_socket_server.cc",
            "unix_socket_client.cc",
            "tcp_server.cc",
            "tcp_client.cc"
        ],
        hdrs = [
            "unix_socket_server.h",
            "unix_socket_client.h",
            "tcp_server.h",
            "tcp_client.h"
        ],
        deps = [
            "//modules/apollo_dttf/common:apollo_dttf_common",
            "//cyber",
        ],
)



apollo_package()
cpplint()

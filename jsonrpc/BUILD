package(default_visibility = ["//visibility:public"])

cc_library(
    name = "jsonrpc_lib",
    srcs = glob(["**/*.cc"]),
    hdrs = glob(["**/*.h"]),
    deps = [
        "@github_nlohmann_json//:json",
        "@com_google_googletest//:gtest",
    ],
    alwayslink = True,
)
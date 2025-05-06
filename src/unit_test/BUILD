package(default_visibility = ["//visibility:public"])

cc_test(
    name = "unit_test",
    srcs = glob([
        "*.cc",
    ]),
    deps = [
        "//json_rpc:json_rpc_lib",
        "@com_google_googletest//:gtest_main",
        "@com_google_googletest//:gtest",
    ],
)
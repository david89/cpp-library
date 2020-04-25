cc_library(
    name = "string_view_lib",
    srcs = ["string_view.cc"],
    hdrs = ["string_view.h"],
    deps = [],
)

cc_test(
    name = "string_view_test",
    srcs = ["string_view_test.cc"],
    deps = [
        ":string_view_lib",
        "@gtest//:gtest_main",
    ],
)

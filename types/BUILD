cc_library(
    name = "optional_lib",
    hdrs = ["optional.h"],
    deps = [
        "//types/internal:enable_copy_move_lib",
    ],
)

cc_test(
    name = "optional_test",
    srcs = ["optional_test.cc"],
    deps = [
        ":optional_lib",
        "@gtest//:gtest_main",
    ],
)

cc_library(
    name = "string_view_lib",
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

# Description: Inference Gateway

package(
    default_visibility = [
        "//tensorflow_serving:internal",
    ],
    features = ["no_layering_check"],
)

licenses(["notice"])  

filegroup(
    name = "all_files",
    srcs = glob(
        ["**/*"],
        exclude = [
            "**/METADATA",
            "**/OWNERS",
        ],
    ),
)

cc_library(
    name = "model",
    srcs = ["model.cc"],
    hdrs = ["model.h"],
    deps = [
        "@org_tensorflow//tensorflow/core:lib",        
    ],
)

cc_library(
    name = "sn",
    srcs = ["sn.cc"],
    hdrs = ["sn.h"],
    deps = [
        ":model",
        "@grpc//:grpc++_unsecure",
        "@protobuf_archive//:protobuf_lite",        
        "@org_tensorflow//tensorflow/core:lib",        
        "//tensorflow_serving/apis:model_management_proto",
        "//tensorflow_serving/apis:model_service_proto",
        "//tensorflow_serving/apis:prediction_service_proto",        
    ],
)

cc_library(
    name = "sn_pool",
    srcs = ["sn_pool.cc"],
    hdrs = ["sn_pool.h"],
    deps = [
        ":sn",
        "@org_tensorflow//tensorflow/core:lib",        
    ],
)


cc_library(
    name = "handles",
    srcs = ["handles.cc"],
    hdrs = ["handles.h"],
    deps = [
        ":model",
        ":sn",
        "@org_tensorflow//tensorflow/core:lib",        
    ],
)


cc_library(
    name = "request_manager",
    srcs = ["request_manager.cc"],
    hdrs = ["request_manager.h"],
    deps = [
        ":handles",
        ":sn_pool",
        "@org_tensorflow//tensorflow/core:lib",        
    ],
)

cc_binary(
    name = "inference_gateway",
    srcs = [
        "main.cc",
    ],
    deps = [
        ":request_manager",
        "@org_tensorflow//tensorflow/core:lib",
                        "@grpc//:grpc++_unsecure",
    ]
)


cc_test(
    name = "handles_test",
    srcs = [
        "handles_test.cc",
    ],
    deps = [
        ":handles",
        "@org_tensorflow//tensorflow/core:lib",
        "@org_tensorflow//tensorflow/core:test",
        "//tensorflow_serving/core/test_util:test_main",
    ],
)


cc_test(
    name = "sn_pool_test",
    srcs = [
        "sn_pool_test.cc",
    ],
    deps = [
        ":sn_pool",
        "@org_tensorflow//tensorflow/core:lib",
        "@org_tensorflow//tensorflow/core:test",
        "//tensorflow_serving/core/test_util:test_main",
    ],
)

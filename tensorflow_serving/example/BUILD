# Description: Tensorflow Serving examples.

package(
    default_visibility = ["//tensorflow_serving:internal"],
    features = ["no_layering_check"],
)

licenses(["notice"])  # Apache 2.0

load("//tensorflow_serving:serving.bzl", "serving_proto_library")

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

py_library(
    name = "mnist_input_data2",
    srcs = ["mnist_input_data2.py"],
    srcs_version = "PY2AND3",
)

py_binary(
    name = "mnist_saved_model2",
    srcs = [
        "mnist_saved_model2.py",
    ],
    srcs_version = "PY2AND3",
    deps = [
        ":mnist_input_data2",
        "@org_tensorflow//tensorflow:tensorflow_py",
    ],
)

py_binary(
    name = "mnist_client2",
    srcs = [
        "mnist_client2.py",
    ],
    srcs_version = "PY2AND3",
    deps = [
        ":mnist_input_data2",
        "//tensorflow_serving/apis:predict_proto_py_pb2",
        "//tensorflow_serving/apis:prediction_service_proto_py_pb2",
        "@org_tensorflow//tensorflow:tensorflow_py",
    ],
)

py_binary(
    name = "inception_saved_model",
    srcs = [
        "inception_saved_model.py",
    ],
    srcs_version = "PY2AND3",
    deps = [
        "@inception_model//inception",
        "@org_tensorflow//tensorflow:tensorflow_py",
    ],
)

py_binary(
    name = "inception_client",
    srcs = [
        "inception_client.py",
    ],
    srcs_version = "PY2AND3",
    deps = [
        "//tensorflow_serving/apis:predict_proto_py_pb2",
        "//tensorflow_serving/apis:prediction_service_proto_py_pb2",
        "@org_tensorflow//tensorflow:tensorflow_py",
    ],
)

cc_binary(
    name = "inception_client_cc",
    srcs = [
        "inception_client.cc",
    ],
    deps = [
        "//tensorflow_serving/apis:prediction_service_proto",
        "@grpc//:grpc++_unsecure",
        "@org_tensorflow//tensorflow/core:framework",
        "@protobuf_archive//:protobuf_lite",
    ],
)

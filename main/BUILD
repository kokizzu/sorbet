config_setting(
    name = "jemalloc",
    values = {
        "define": "jemalloc=true",
        "compilation_mode": "opt",
    },
    visibility = ["//visibility:public"],
)

config_setting(
    name = "mimalloc",
    values = {
        "define": "mimalloc=true",
        "compilation_mode": "opt",
    },
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "sorbet",
    srcs = [
        "main.cc",
    ],
    linkstatic = select({
        "//tools/config:linkshared": 0,
        "//conditions:default": 1,
    }),
    malloc = select({
        ":jemalloc": "@jemalloc//:jemalloc",
        ":mimalloc": "@mimalloc//:mimalloc",
        "//conditions:default": "@bazel_tools//tools/cpp:malloc",
    }),
    visibility = ["//visibility:public"],
    deps = [
        "realmain",
        "//main/pipeline/semantic_extension:none",
        "//payload",
    ],
)

cc_binary(
    name = "sorbet-orig",
    srcs = [
        "main.cc",
    ],
    linkstatic = select({
        "//tools/config:linkshared": 0,
        "//conditions:default": 1,
    }),
    visibility = ["//visibility:public"],
    deps = [
        "realmain-orig",
        "//main/pipeline/semantic_extension:none",
        "//payload:text",
    ],
)

cc_library(
    name = "realmain",
    srcs = [
        "realmain.cc",
        "realmain.h",
    ],
    linkstatic = select({
        "//tools/config:linkshared": 0,
        "//conditions:default": 1,
    }),
    visibility = ["//visibility:public"],
    deps = [
        "//ast",
        "//common/statsd",
        "//core",
        "//core/proto",
        "//hashing",
        "//main/autogen",
        "//main/cache",
        "//main/lsp",
        "//main/minimize",
        "//main/options",
        "//main/pipeline",
        "//payload:interface",
    ],
)

cc_library(
    name = "realmain-orig",
    srcs = [
        "realmain.cc",
        "realmain.h",
    ],
    defines = ["SORBET_REALMAIN_MIN"],
    linkstatic = select({
        "//tools/config:linkshared": 0,
        "//conditions:default": 1,
    }),
    visibility = ["//visibility:public"],
    deps = [
        "//ast",
        "//core",
        "//hashing:hashing-orig",
        "//main/cache:cache-orig",
        "//main/options",
        "//main/pipeline:pipeline-orig",
        "//payload:interface",
    ],
)

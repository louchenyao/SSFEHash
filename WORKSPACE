load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")

git_repository(
    name = "gtest",
    remote = "https://github.com/google/googletest",
    tag = "release-1.8.1",
)

git_repository(
    name = "benchmark",
    remote = "https://github.com/google/benchmark",
    tag = "v1.5.0",
)

new_git_repository(
    name = "perfevent",
    remote = "https://github.com/viktorleis/perfevent.git",
    commit = "f34b43af69e45521b973b4065e25504c3ec43d93",
    build_file_content = """
cc_library(
    name = "all",
    hdrs = ["PerfEvent.hpp"],
    visibility = ["//visibility:public"],
)""",
)

new_git_repository(
    name = "smhasher",
    remote = "https://github.com/aappleby/smhasher.git",
    commit = "61a0530f28277f2e850bfc39600ce61d02b518de",
    build_file_content = """
cc_library(
    name = "murmurhash3",
    hdrs = ["src/MurmurHash3.h"],
    srcs = ["src/MurmurHash3.cpp"],
    strip_include_prefix = "src",
    visibility = ["//visibility:public"],
)""",
)

new_git_repository(
    name = "xxhash",
    remote = "https://github.com/Cyan4973/xxHash.git",
    commit = "e2f4695899e831171ecd2e780078474712ea61d3",
    build_file_content = """
cc_library(
    name = "xxhash",
    hdrs = ["xxhash.h", "xxh3.h"],
    srcs = ["xxhash.c"],
    copts = ["-O3", "-march=native", "-Wall", "-Wextra", "-Werror"],
    visibility = ["//visibility:public"],
)""",
)

local_repository(
    name = "dpdk",
    path = "dpdk",
)

local_repository(
    name = "othello",
    path = "othello",
)

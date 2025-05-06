load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def clean_dep(dep):
    return str(Label(dep))

def jsonrpc_workspace(path_prefix = "", tf_repo_name = "", **kwargs):

    github_nlohmann_json_ver = kwargs.get("github_nlohmann_json_ver", "3.6.1")
    github_nlohmann_json_sha256 = kwargs.get("github_nlohmann_json_sha256", "2a515568e5b09babf7277f7d9ff2e2ae8ad6c94ec647e38e8b174628733dc2f5")
    github_nlohmann_json_urls = [
        "https://github.com/nlohmann/json/archive/refs/tags/{ver}.tar.gz".format(ver = github_nlohmann_json_ver),
    ]
    http_archive(
        name = "github_nlohmann_json",
        sha256 = github_nlohmann_json_sha256,
        strip_prefix = "json-{ver}".format(ver = github_nlohmann_json_ver),
        build_file = clean_dep("//third_party/nlohmann_json:BUILD"),
        urls = github_nlohmann_json_urls,
    )

    com_google_googletest_ver = kwargs.get("com_google_googletest_ver", "1.10.0")
    com_google_googletest_sha256 = kwargs.get("com_google_googletest_sha256", "9dc9157a9a1551ec7a7e43daea9a694a0bb5fb8bec81235d8a1e6ef64c716dcb")
    com_google_googletest_urls = [
        "https://github.com/google/googletest/archive/release-{ver}.tar.gz".format(ver = com_google_googletest_ver),
    ]
    http_archive(
        name = "com_google_googletest",
        sha256 = com_google_googletest_sha256,
        strip_prefix = "googletest-release-{ver}".format(ver = com_google_googletest_ver),
        build_file = clean_dep("//third_party/gtest:BUILD"),
        urls = com_google_googletest_urls,
    )
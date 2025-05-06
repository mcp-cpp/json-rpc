cmake_minimum_required(VERSION 3.10)
project(mcp)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 设置包含目录
include_directories(${CMAKE_SOURCE_DIR})

# 递归查找 mcp 文件夹及其子文件夹下的所有 .cc 和 .h 文件
file(GLOB_RECURSE SOURCES_MCP "${CMAKE_SOURCE_DIR}/mcp/**/*.cc")
file(GLOB_RECURSE HEADERS_MCP "${CMAKE_SOURCE_DIR}/mcp/**/*.h")

# 递归查找 examples 文件夹及其子文件夹下的所有 .cc 和 .h 文件
file(GLOB_RECURSE SOURCES_EXAMPLES "${CMAKE_SOURCE_DIR}/examples/**/*.cc")
file(GLOB_RECURSE HEADERS_EXAMPLES "${CMAKE_SOURCE_DIR}/examples/**/*.h")

# 合并所有源文件和头文件
set(SOURCES ${SOURCES_MCP} ${SOURCES_EXAMPLES})
set(HEADERS ${HEADERS_MCP} ${HEADERS_EXAMPLES})

# 添加 nlohmann/json 库
add_library(json_lib INTERFACE)
target_include_directories(json_lib INTERFACE ${CMAKE_SOURCE_DIR}/third_party/json.hpp)

# 添加可执行文件
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
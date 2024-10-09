cmake_minimum_required(VERSION 3.13)
project(pinpoint_http_test_server)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
# FetchContent_Declare(
#   pinpoint
#   GIT_REPOSITORY https://github.com/pinpoint-apm/pinpoint-c-agent.git
#   # GIT_TAG        74bc39d813d664cb56b78b1506d91932c8131396 
#   # not recommended, please use hash key like `74bc39d813d664cb56b78b1506d91932c8131396`or tag name
#   GIT_TAG        v0.5.0
# )

FetchContent_Declare(pinpoint SOURCE_DIR ${PROJECT_SOURCE_DIR}/../../)

FetchContent_GetProperties(pinpoint)
if (NOT pinpoint_POPULATED)
    FetchContent_Populate(pinpoint)
    add_subdirectory(${pinpoint_SOURCE_DIR}/common ${pinpoint_BINARY_DIR})
endif ()

FetchContent_Declare(
  httplib
  URL https://github.com/yhirose/cpp-httplib/archive/refs/tags/v0.18.0.tar.gz
)

FetchContent_MakeAvailable(httplib pinpoint)


add_executable(server server.cpp)

target_link_libraries(server PRIVATE pthread  pinpoint_common_static httplib rt)
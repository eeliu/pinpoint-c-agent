cmake_minimum_required(VERSION 3.0)
project(common)
set(CMAKE_CXX_STANDARD 11)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include_directories("json/include"
                    "include"
                    "src")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Werror -g3 -O0 -fPIC")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -fPIC")

enable_testing()
find_package(GTest REQUIRED)

find_package(pcre)

add_subdirectory(json)
add_subdirectory(src)
add_subdirectory(test)

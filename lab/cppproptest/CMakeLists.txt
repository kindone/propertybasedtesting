# Standalone sandbox: link against cppproptest2 (proptest) for experiments and PoCs.
#
# cppproptest2 is fetched with FetchContent (see cache variables below). Configure:
#   cmake -S . -B build
#
# Pin a commit for reproducibility:
#   cmake -S . -B build -DCPPPROPTEST_GIT_TAG=a1b2c3d4

cmake_minimum_required(VERSION 3.21)
project(cppproptest_lab LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CPPPROPTEST_GIT_REPOSITORY "https://github.com/kindone/cppproptest2.git" CACHE STRING
    "Git URL for cppproptest2 (FetchContent)")
set(CPPPROPTEST_GIT_TAG "main" CACHE STRING
    "Git branch, tag, or commit hash for cppproptest2")

include(FetchContent)
FetchContent_Declare(
    cppproptest2
    GIT_REPOSITORY "${CPPPROPTEST_GIT_REPOSITORY}"
    GIT_TAG "${CPPPROPTEST_GIT_TAG}"
    GIT_SHALLOW TRUE
)

FetchContent_GetProperties(cppproptest2)
if(NOT cppproptest2_POPULATED)
    FetchContent_Populate(cppproptest2)
    add_subdirectory(
        "${cppproptest2_SOURCE_DIR}"
        "${CMAKE_BINARY_DIR}/_deps/cppproptest2-build"
        EXCLUDE_FROM_ALL
    )
endif()

add_executable(poc_basic src/poc_basic.cpp)
target_compile_features(poc_basic PRIVATE cxx_std_23)
target_include_directories(poc_basic PRIVATE "${cppproptest2_SOURCE_DIR}")
target_link_libraries(poc_basic PRIVATE proptest)

get_target_property(_proptest_type proptest TYPE)
if(_proptest_type STREQUAL SHARED_LIBRARY)
    target_compile_definitions(poc_basic PRIVATE PROPTEST_DLL)
endif()

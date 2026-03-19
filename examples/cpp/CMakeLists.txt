CMAKE_MINIMUM_REQUIRED(VERSION 3.15)
PROJECT(proptest_examples VERSION 1.0)

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Fetch cppproptest2 from GitHub
FetchContent_Declare(
    cppproptest2
    GIT_REPOSITORY https://github.com/kindone/cppproptest2.git
    GIT_TAG main
)
FetchContent_MakeAvailable(cppproptest2)

# Fetch GoogleTest (same version cppproptest2 uses)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Sample property test executable
ADD_EXECUTABLE(sample_proptest sample_proptest.cpp)
TARGET_INCLUDE_DIRECTORIES(sample_proptest PRIVATE ${cppproptest2_SOURCE_DIR})
TARGET_LINK_LIBRARIES(sample_proptest PRIVATE proptest gtest_main)
ADD_TEST(NAME sample_proptest COMMAND sample_proptest)

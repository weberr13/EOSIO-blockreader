cmake_minimum_required (VERSION 3.0)
ENABLE_LANGUAGE(CXX)

project (EOSIO-blockreader)
set(PROJECT_SRC ${EOSIO-blockreader_SOURCE_DIR}/src)
set(TEST_SRC ${EOSIO-blockreader_SOURCE_DIR}/test)
set(THIRDPARTY_SRC ${EOSIO-blockreader_SOURCE_DIR}/thirdParty)
SET(PLATFORM_LINK_LIBRARIES rt)
SET(CMAKE_CXX_FLAGS "-Wall -rdynamic -Wunused -std=c++14 -pthread -D_GLIBCXX_USE_NANOSLEEP")

file(GLOB SRC_FILES ${EOSIO-blockreader_SOURCE_DIR}/src/*.h ${EOSIO-blockreader_SOURCE_DIR}/src/*.hpp ${EOSIO-blockreader_SOURCE_DIR}/src/*.cpp)

include_directories(${PROJECT_SRC})
add_executable(blockreader ${SRC_FILES})
target_link_libraries (blockreader curl)
SET(VERSION_STRING ${VERSION})

set(GTEST_DIR ${THIRDPARTY_SRC}/googletest-release-1.8.0/googletest)
set(GTEST_INCLUDE_DIRECTORIES ${EOSIO-blockreader_SOURCE_DIR}/thirdParty/googletest-release-1.8.0/googletest/include/ ${EOSIO-blockreader_SOURCE_DIR}/thirdParty/googletest-release-1.8.0/googletest ${GTEST_DIR}/src)
include_directories(${GTEST_INCLUDE_DIRECTORIES})
add_library(gtest_180_lib ${EOSIO-blockreader_SOURCE_DIR}/thirdParty/googletest-release-1.8.0/googletest/src/gtest-all.cc)
set_target_properties(gtest_180_lib PROPERTIES COMPILE_DEFINITIONS "GTEST_HAS_RTTI=0")
enable_testing(true)
include_directories(test)
include_directories(${PROJECT_SRC})
file(GLOB TEST_SRC_FILES "test/*.cpp")
SET(CMAKE_CXX_FLAGS "-Wall -Wno-sign-compare -Wunused -std=c++14 -pthread -D_GLIBCXX_USE_NANOSLEEP")

add_executable (UnitTests thirdParty/test_main.cpp ${TEST_SRC_FILES})
target_link_libraries (UnitTests gtest_180_lib)

set_target_properties(${test} PROPERTIES COMPILE_FLAGS "-isystem -pthread ")


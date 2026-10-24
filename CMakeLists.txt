cmake_minimum_required(VERSION 3.20)
project(LRU_cache LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Library from src/, exposing headers/ as its public include root.
add_library(LRU_cache_lib
    ${CMAKE_CURRENT_SOURCE_DIR}/src/LRUCache.cpp
)
target_include_directories(LRU_cache_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/headers
)

# Executable: main.cpp + the library.
add_executable(LRU_cache_app
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)
target_link_libraries(LRU_cache_app PRIVATE LRU_cache_lib)

# Tests.
enable_testing()
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(LRU_cache_tests
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/LRUCacheTest.cpp
)
target_link_libraries(LRU_cache_tests PRIVATE LRU_cache_lib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(LRU_cache_tests)
cmake_minimum_required(VERSION 3.10)
project(gpAnalizer_tests CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------------
# Dependencies
# ---------------------------------------------------------------------------
find_package(OpenCV REQUIRED core imgproc imgcodecs highgui)

find_library(TINYXML2_LIB NAMES tinyxml2 libtinyxml2
    HINTS /usr/lib/x86_64-linux-gnu /usr/lib /usr/local/lib)
if(NOT TINYXML2_LIB)
    # Fallback: try finding the versioned .so directly
    find_file(TINYXML2_LIB libtinyxml2.so.10
        HINTS /usr/lib/x86_64-linux-gnu /usr/lib)
endif()
if(NOT TINYXML2_LIB)
    message(FATAL_ERROR "tinyxml2 library not found")
endif()

find_library(GTEST_LIB     gtest)
find_library(GTEST_MAIN_LIB gtest_main)
if(NOT GTEST_LIB OR NOT GTEST_MAIN_LIB)
    message(FATAL_ERROR "Google Test libraries not found. "
            "Install libgtest-dev and build the static libraries.")
endif()

find_package(Threads REQUIRED)

# ---------------------------------------------------------------------------
# Source files under test (no Qt dependency)
# ---------------------------------------------------------------------------
set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/..")

set(NODE_SOURCES
    ${ROOT_DIR}/node/node.cpp
    ${ROOT_DIR}/node/nodeid.cpp
    ${ROOT_DIR}/node/functionnode.cpp
    ${ROOT_DIR}/node/morphonode.cpp
    ${ROOT_DIR}/node/threshnode.cpp
    ${ROOT_DIR}/node/terminalnode.cpp
    ${ROOT_DIR}/node/functionset.cpp
)

set(FITNESS_SOURCES
    ${ROOT_DIR}/fitness/fitness.cpp
    ${ROOT_DIR}/fitness/hamming.cpp
    ${ROOT_DIR}/fitness/hausdorff.cpp
    ${ROOT_DIR}/fitness/hausdorffcanny.cpp
    ${ROOT_DIR}/fitness/hausdorffsmall.cpp
    ${ROOT_DIR}/fitness/hausdorffmodified.cpp
    ${ROOT_DIR}/fitness/fitnessgenerator.cpp
    ${ROOT_DIR}/generator/nodegenerator.cpp
)

# ---------------------------------------------------------------------------
# Test executable
# ---------------------------------------------------------------------------
add_executable(gpAnalizer_tests
    test_nodes.cpp
    test_fitness.cpp
    ${NODE_SOURCES}
    ${FITNESS_SOURCES}
)

target_include_directories(gpAnalizer_tests PRIVATE
    ${ROOT_DIR}
    ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(gpAnalizer_tests
    ${OpenCV_LIBS}
    ${TINYXML2_LIB}
    ${GTEST_LIB}
    ${GTEST_MAIN_LIB}
    Threads::Threads
)

# ---------------------------------------------------------------------------
# CTest integration
# ---------------------------------------------------------------------------
enable_testing()
add_test(NAME gpAnalizer_tests COMMAND gpAnalizer_tests)

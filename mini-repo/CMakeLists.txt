cmake_minimum_required(VERSION 3.16)
project(training_compile_check CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Lets editors (VS Code C/C++ extension, clangd, CLion) resolve the mock
# includes and stop showing "cannot find mbed.h" squiggles. Without this,
# the code still compiles fine from the command line -- squiggles are an
# editor-indexing problem, not a build problem -- but trainees will see red
# underlines everywhere and assume something's broken.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---------------------------------------------------------------------------
# This build never touches the ARM toolchain or real mbed-os. It compiles
# with whatever host compiler CMake finds (MSVC on Windows, AppleClang on
# macOS, GCC on WSL/Linux) and only checks that trainee code TYPE-CHECKS --
# it does not produce anything that flashes to a robot.
# ---------------------------------------------------------------------------

# 1) Our host stand-in for the mbed API surface.
include_directories(${CMAKE_SOURCE_DIR}/mbed-os)

# 2) Project-root include path so repo-relative includes like
#    "base_robot/BaseRobot.h" and "util/..." resolve correctly.
include_directories(${CMAKE_SOURCE_DIR})

# 3) Real, unmodified project headers for whatever the trainee is
#    implementing against (ChassisSubsystem.h, PID.h, DJIMotor.h, ...).
#    Copy these in from your real repo as-is -- do NOT mock these, only the
#    hardware leaf headers in mock_mbed/ need faking.
include_directories(${CMAKE_SOURCE_DIR}/core)
include_directories(${CMAKE_SOURCE_DIR}/base_robot)
include_directories(${CMAKE_SOURCE_DIR}/robots)

# ---------------------------------------------------------------------------
# Self-check: proves the mock satisfies real mbed usage patterns. Every
# trainee should be able to build and run this with zero setup beyond a
# compiler + CMake.
# ---------------------------------------------------------------------------
add_executable(self_check robots/Infantry/self_check.cpp)

# Try compiling the actual robot translation unit in host mode.
# This is intentionally a compile-only check; if the source relies on real
# hardware-specific mbed-os details, the compiler should report the real gap.
add_library(infantry_check OBJECT robots/Infantry/Infantry.cpp)

# ---------------------------------------------------------------------------
# Trainee submissions: add one OBJECT library per file you want checked.
# OBJECT libraries compile to a .o/.obj but are never linked into a runnable
# binary here, so trainees don't need working implementations of every
# hardware-touching class -- only the headers need to resolve and the types
# need to line up. This is exactly the "does it compile" bar you want.
#
# Example, once you've copied in ChassisSubsystem.h to real_headers/:
#
# add_library(chassis_check OBJECT ChassisSubsystem.cpp)
#
# Build just that target to check just that file:
#   cmake --build build --target chassis_check
# ---------------------------------------------------------------------------

# If you're poking around here, you're either a nerd, or completely lost
# Assuming you're the former, this file helps us manage our builds
cmake_minimum_required(VERSION 3.16)

# This sets the projects internal name (we won't ever see it)
# The CXX tag just means that we're in C++
project(training_compile_check CXX)

# These prevent our outputs from cluttering up our directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# This lets our IDE figure out how the #includes work, so stops us from seeing
# erroneous squiggles under everything
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Our LARP embedOS
include_directories(${CMAKE_SOURCE_DIR}/mbed-os)

# These directories make it easier to do #includes, so that we can just put
# the final parts and not do the whole thing 
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/mbed-os/drivers)
include_directories(${CMAKE_SOURCE_DIR}/core/subsystems)
include_directories(${CMAKE_SOURCE_DIR}/core)
include_directories(${CMAKE_SOURCE_DIR}/base_robot)
include_directories(${CMAKE_SOURCE_DIR}/robots)

# This basically declares what exactly we're gonna be doing
# As you can see, we're going to be using Infantry.cpp, and making
# an object (.o), as opposed to other types such as static, shared, or interface

add_library(infantry_check OBJECT robots/Infantry/Infantry.cpp)

# Don't worry, you don't need to know these details (I didn't before
# I wrote this). I just put these here if you're curious. Let me 
# know if you actually took the time to read this lol, I don't 
# expect the trainees to poke around random files like this 
# - Dil 
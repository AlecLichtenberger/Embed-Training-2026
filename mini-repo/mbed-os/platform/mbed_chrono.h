#pragma once
#include <chrono>

// Real mbed-os does this so driver code can write chrono::microseconds
// instead of std::chrono::microseconds. Global scope is intentional.
namespace chrono = std::chrono;
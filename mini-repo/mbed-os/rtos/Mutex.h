#pragma once

namespace rtos {

class Mutex {
public:
    Mutex() = default;
    ~Mutex() = default;
    Mutex(const Mutex&) = delete;
    Mutex& operator=(const Mutex&) = delete;

    void lock() {}
    void unlock() {}
    bool trylock() { return true; }
};

} // namespace rtos

using rtos::Mutex;
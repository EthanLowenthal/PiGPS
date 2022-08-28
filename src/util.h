#include <chrono>

#ifndef UTIL_H
#define UTIL_H

double get_current_ms() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
                       std::chrono::system_clock::now().time_since_epoch())
                       .count();
}

#endif
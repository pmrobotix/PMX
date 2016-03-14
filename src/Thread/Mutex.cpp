#include "Mutex.hpp"

#include <stddef.h>

utils::Mutex::Mutex() {
    pthread_mutex_init(& mutex_, NULL);
}

void utils::Mutex::lock() {
    pthread_mutex_lock(& mutex_);
}

void utils::Mutex::unlock() {
    pthread_mutex_unlock(& mutex_);
}

bool utils::Mutex::tryLock() {
    int value = pthread_mutex_trylock(& mutex_);
    return value == 0;
}

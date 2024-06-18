#pragma once
#include<pthread.h>
#include"NoCopyable.h"

class MutexLock
:public NonCopyable{
public:
    MutexLock();
    ~MutexLock();

    void lock();
    void unlock();
    void trylock();

    pthread_mutex_t *getMutexLockPtr(){
        return &_mutex;
    }

private:
    pthread_mutex_t _mutex;
};

//自动管理锁
class MutexLockGuard{
public:
    MutexLockGuard(MutexLock &mutex):_mutex(mutex){
        _mutex.lock();
    }

    ~MutexLockGuard(){
        _mutex.unlock();
    }
private:
    MutexLock &_mutex;
};

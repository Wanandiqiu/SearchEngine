#pragma once
#include<pthread.h>
#include"MutexLock.h"
#include"NoCopyable.h"

class Condition
:public NonCopyable{
public:
    Condition(MutexLock &mutex);
    ~Condition();
    
    void wait();
    void notify();
    void notifyAll();

private:
    MutexLock &_mutex;
    pthread_cond_t _cond;
};


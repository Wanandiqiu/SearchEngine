#pragma once
#include <pthread.h>
#include <iostream>

class Thread
{
public:
    Thread();
    virtual ~Thread();

    void start();
    void stop();

private:
    // 在 PthredFunc 中执行 run 函数

    static void *PthredFunc(void*);     // 线程入口函数，因为是成员函数，设置为static

    virtual void run() = 0; // 线程要执行的任务

    pthread_t _thid;
    bool _isRunning;
};
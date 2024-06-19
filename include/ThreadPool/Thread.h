#pragma once
#include <pthread.h>
#include <iostream>
#include <functional>

// 任务可以通过参数传递进来，交给数据成员
class Thread
{
public:
    using ThreadCallback = std::function<void()>;   // 任务类型
    Thread(ThreadCallback &&cb);
    virtual ~Thread();

    void start();
    void stop();

private:
    // 在 PthredFunc 中执行 run 函数

    static void *PthredFunc(void*);     // 线程入口函数，因为是成员函数，设置为static

    pthread_t _thid;
    bool _isRunning;
    ThreadCallback _cb;
};
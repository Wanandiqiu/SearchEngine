#pragma once
#include "TaskQueue.h"
#include <unistd.h>
#include "Thread.h"
#include <vector>
#include <memory>
#include <functional>
#include <vector>

using Task = std::function<void()>;

class ThreadPool
{
    
public:
    ThreadPool(size_t threadNum, size_t queSize);
    ~ThreadPool();

    void start();
    void stop();

    void AddTask(Task &&taskcb);   // 添加任务

    void doWork();     // 线程池交给工作线程的任务
    
    Task getTask();
private:

    size_t _threadNum;  // 线程的数量
    size_t _queSize;    
    TaskQueue _que;
    std::vector<std::unique_ptr<Thread>> _threads;  // 存放工作线程
    bool _isRunning;    
};

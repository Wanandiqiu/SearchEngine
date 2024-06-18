#pragma once
#include "MutexLock.h"
#include "Condition.h"
#include "Task.h"
#include <queue>

/*
    任务队列，队列中的任务用一个类 Task 来表示
*/

using ElemType = Task*;

class TaskQueue
{
public:
    TaskQueue(int qsize);
    ~TaskQueue();

    bool empty();
    bool full();

    void push(ElemType data);
    ElemType pop();

private:
    size_t _queSize;    // 任务队列大小
    std::queue<ElemType> _que;  // 任务队列
    MutexLock _mutex;
    Condition _Full;    // 满
    Condition _Empty;   // 空
};
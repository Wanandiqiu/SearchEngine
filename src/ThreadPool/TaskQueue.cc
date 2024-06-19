#include "TaskQueue.h"

TaskQueue::TaskQueue(int qsize):
_queSize(qsize), _que(), _mutex(), _Full(_mutex), _Empty(_mutex)
{

}

TaskQueue::~TaskQueue()
{

}

bool TaskQueue::empty()
{
    return 0 == _que.size();
}

bool TaskQueue::full()
{
    return _que.size() == _queSize;
}

// 生产数据
void TaskQueue::push(ElemType &&data)
{
    MutexLockGuard guard(_mutex);

    while(full())       // 虚假唤醒
    {
        // 队列满，等着去吧
        _Empty.wait();
    }

    _que.push(std::move(data));
    
    _Full.notify(); // 唤醒消费者
}

// 消费资源
ElemType TaskQueue::pop()
{
    MutexLockGuard guard(_mutex);

    while(empty())
    {
        // 队列空，没得消费
        _Full.wait();
    }

    ElemType data = _que.front();
    _que.pop();

    _Empty.notify();    // 唤醒生产者

    return data;
}


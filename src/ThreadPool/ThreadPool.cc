#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t threadNum, size_t queSize):
_threadNum(threadNum), _queSize(queSize), _que(_queSize), _isRunning(false)
{
    _threads.reserve(threadNum);
}

ThreadPool::~ThreadPool()
{
    if(!_isRunning)
    {
        stop();
    }
}

void ThreadPool::start()
{
    for(size_t idx = 0; idx < _threadNum; idx++)
    {
        std::unique_ptr<Thread> up(new Thread(std::bind(&ThreadPool::doWork, this)));   // 注册
        _threads.push_back(std::move(up));
    }

    for(auto &elem : _threads)
    {
        elem->start();  // 每个线程跑起来
    }

}


void ThreadPool::stop()
{
    // 线程池退出
    // 任务队列中有数据，不该退出
    
    _isRunning = false;

    for(auto &elem : _threads)
    {
        elem->stop();
    }
}

void ThreadPool::AddTask(Task &&taskcb)
{
    if(taskcb)
    {
        _que.push(std::move(taskcb));
    }
}

Task ThreadPool::getTask()
{
    return _que.pop();
}

//线程池交给工作线程具体逻辑
void ThreadPool::doWork(){
    
    //如果process执行的比较慢的话，那么_isExit有足够时间设置为true，进不到while
    //不会调用getTask，不会调用_taskQue.pop()，也不会让_notEmpty.wait()执行
    //程序不会卡死，如果process执行的很快，_isExit来不及设置为true，进入while
    //此时没有任务，getTask会调用pop，会卡在_notEmpty.wait

    //只要任务队列中有任务，工作线程就会执行任务
    while(!_isRunning){
    //获取任务
        Task taskcb = getTask();    //pop  wait
        if(taskcb){
        //执行具体操作
            taskcb();   // 执行回调
            //sleep(2);
        }
    }
}


#include "Thread.h"

Thread::Thread():_thid(0), _isRunning(false)
{

}

Thread::~Thread()
{
    if(_isRunning)
    {
        pthread_detach(_thid);  // 不需要等待线程完成的场景。线程完成后会自动释放资源
        _isRunning = false;
    }
}

void Thread::start()
{
    if(_isRunning == false)
    {
        int ret = pthread_create(&_thid, nullptr, PthredFunc, this);
        if(ret == -1)
        {
            std::cout << "pthread_create error" << std::endl;
            return;
        }
    }
    _isRunning = true;
}

void Thread::stop()
{
    if(_isRunning == true)
    {
        int ret = pthread_join(_thid, nullptr);     // 需要等待线程完成并获取其返回值, 确保子线程执行完毕后再进行下一步操作
        if(ret == -1)
        {
            std::cout << "pthread_join error" << std::endl;
            return;
        }
    }
    _isRunning = false;
}


void *Thread::PthredFunc(void* arg)
{
    // 再次执行 run 函数，静态函数不能调用非静态方法 run , 所以 this 作为 pthread_create 的第四个参数
    Thread* pt = static_cast<Thread*>(arg);
    if(pt)
    {
        pt->run();
    }
    pthread_exit(nullptr);  // 线程完成其任务时终止线程的执行, 可以返回一个值给 pthread_join
}
#if 0
#include "Thread.h"
#include "ThreadPool.h"
#include <iostream>
#include <memory>

class MyTask
{
public:
    void process() 
    {
        std::cout << "hello world" << std::endl;
    }

};

int main()
{
    // 创建Task对象
    std::unique_ptr<MyTask> up(new MyTask());

    ThreadPool pool(4, 10);
    pool.start();

    // 添加任务
    size_t idx = 10;
    while(idx-- > 0)
    {
        pool.AddTask(std::bind(&MyTask::process, up.get()));
        std::cout << "111" << std::endl;
    }

    pool.stop();

    return 0;
}
#endif


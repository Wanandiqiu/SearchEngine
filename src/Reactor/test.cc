#include <iostream>
#include"EventLoop.h"
#include"TcpConnection.h"
#include"ThreadPool.h"
#include<string>
#include"Acceptor.h"
#include"TcpServer.h"

using std::string;
using std::cout;
using std::endl;

ThreadPool *gPool = nullptr;    // 全局变量，onMessage 中能用到

class MyTask{
public:
    MyTask(const string &msg, const TcpConnectionPtr &con):_msg(msg), _con(con){

    }

    void process()
    {
        //业务逻辑
        _msg += "hello";

        _con->sendInLoop(_msg);
    }
private:
    std::string _msg;
    TcpConnectionPtr _con;
};


void onConnection(const TcpConnectionPtr &con){
    cout << con->toString() << " has connected !" << endl;
}

void onMessage(const TcpConnectionPtr &con){
    //接受客户端数据
    string msg = con->receive();
    cout << ">>recv msg from client : " << msg << endl;

    MyTask task(msg, con);   //业务逻辑打个包，传到MyTask中，交给process处理
    gPool->AddTask(std::bind(&MyTask::process, task)); //任务添加到线程池 , bind的值传递与地址传递 
    /*
        bind 中参数用值传递时，数据是拷贝过去的，当 task 生命周期结束时，拷贝的内容还在，即不用担心生命周期的问题
    */
}

void onClose(const TcpConnectionPtr &con){
    cout << con->toString() << " has closed !" << endl;
}


int main()
{   
    ThreadPool pool(4, 10);
    pool.start();
    gPool = &pool;
   
   TcpServer _server("127.0.0.1", 8888);
   _server.setAllCallback(onConnection, onMessage, onClose);

   _server.start();

    return 0;
}


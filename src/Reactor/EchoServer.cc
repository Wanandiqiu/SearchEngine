#include "EchoServer.h"

EchoServer::EchoServer(size_t threadNum, size_t queSize, const string &ip, unsigned short port):
_pool(threadNum, queSize), _server(ip, port)
{

}

EchoServer::~EchoServer()
{

}

void EchoServer::start()
{
    _pool.start();

    using namespace std::placeholders;

    _server.setAllCallback(std::bind(&EchoServer::onConnection, this, _1), 
                            std::bind(&EchoServer::onMessage, this, _1),
                            std::bind(&EchoServer::onClose, this, _1));

    _server.start();
}

void EchoServer::stop()
{
    _pool.stop();
    _server.stop();
}

void EchoServer::onConnection(const TcpConnectionPtr &con)
{
    std::cout << con->toString() << " has connected !" << std::endl;
}

void EchoServer::onMessage(const TcpConnectionPtr &con)
{
    //接受客户端数据
    string msg = con->receive();
    std::cout << ">>recv msg from client : " << msg << std::endl;

    MyTask task(msg, con);   //业务逻辑打个包，传到MyTask中，交给process处理
    _pool.AddTask(std::bind(&MyTask::process, task)); //任务添加到线程池 , bind的值传递与地址传递 
    /*
        bind 中参数用值传递时，数据是拷贝过去的，当 task 生命周期结束时，拷贝的内容还在，即不用担心生命周期的问题
    */
}

void EchoServer::onClose(const TcpConnectionPtr &con)
{
    std::cout << con->toString() << " has closed !" << std::endl;
}
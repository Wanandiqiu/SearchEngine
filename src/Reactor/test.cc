#include <iostream>
#include"EventLoop.h"
#include"TcpConnection.h"
#include<string>
#include"Acceptor.h"

using std::string;
using std::cout;
using std::endl;

void onConnection(const TcpConnectionPtr &con){
    cout << con->toString() << " has connected !" << endl;
}

void onMessage(const TcpConnectionPtr &con){
    //接受客户端数据
    string msg = con->receive();
    cout << ">>recv msg from client : " << msg << endl;

    //发送数据给客户端
    con->send(msg);
}

void onClose(const TcpConnectionPtr &con){
    cout << con->toString() << " has closed !" << endl;
}


int main()
{
    Acceptor acceptor("127.0.0.1", 8888);
    acceptor.ready();   //服务器处于监听

    EventLoop loop(acceptor);

    loop.setConnectCallback(std::move(onConnection));
    loop.setMessageCallback(std::move(onMessage));
    loop.setCloseCallback(std::move(onClose));

    loop.loop();

    return 0;
}


#pragma once
#include "ThreadPool.h"
#include "TcpServer.h"
#include "TcpConnection.h"
#include <iostream>

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

class EchoServer{
public:
    EchoServer(size_t threadNum, size_t queSize, const string &ip, unsigned short port);
    ~EchoServer();

    void start();
    void stop();

    void onConnection(const TcpConnectionPtr &con);
    void onMessage(const TcpConnectionPtr &con);
    void onClose(const TcpConnectionPtr &con);
private:
    ThreadPool _pool;
    TcpServer _server;
};
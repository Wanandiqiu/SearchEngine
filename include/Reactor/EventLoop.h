#pragma once
#include<vector>
#include<memory>
#include<sys/epoll.h>
#include<iostream>
#include<functional>
#include "TcpConnection.h"
#include "Acceptor.h"
#include <functional>
#include <unordered_map>
#include <unistd.h>

class EventLoop{
    using TcpConnectionPtr = std::shared_ptr<TcpConnection>;    // TcpConnection 要保持唯一，所以用智能指针包起来
    using TcpConnectionCallback = std::function<void(const TcpConnectionPtr &con)>;  //回调函数类型
public:
    EventLoop(Acceptor &acceptor);
    ~EventLoop();

    void loop();
    void unloop();

    //EventLoop作为中转，需要将三个回调函数进行注册，然后交给TcpConnection
    //但是EventLoop不负责回调函数的执行，因为EventLoop只有中转的作用，没有执行的能力
    //连接肯定是与TcpConnection相关的
    void setConnectCallback(TcpConnectionCallback &&cb);
    void setMessageCallback(TcpConnectionCallback &&cb);
    void setCloseCallback(TcpConnectionCallback &&cb);

private:
    void WaitEpollFd();
    int CreateEpollFd();

    void AddEpollFd(int fd);
    void DelEpollFd(int fd);

    void handleNewConnection();     // 处理连接建立
    void handleMessage(int fd);   // 处理消息的收发

private:
    int _epfd;  // epoll_create 创建的fd
    Acceptor &_acceptor;
    bool _isLooping;
    std::vector<struct epoll_event> _evList;
    std::unordered_map<int, TcpConnectionPtr> _conns;

    TcpConnectionCallback _onConnectionCb;
    TcpConnectionCallback _onMessageCb;
    TcpConnectionCallback _onCloseCb;

};


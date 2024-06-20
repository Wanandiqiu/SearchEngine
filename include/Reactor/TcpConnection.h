#pragma once
#include<string>
#include"SocketIO.h"
#include"InetAddress.h"
#include"Socket.h"
#include<memory>
#include<functional>

using std::function;
using std::shared_ptr;
using std::string;

class TcpConnection;

using TcpConnectionPtr = shared_ptr<TcpConnection>;

class TcpConnection
:public std::enable_shared_from_this<TcpConnection>{

using TcpConnectionCallback = function<void(const TcpConnectionPtr &con)>;  //回调函数类型

public:
    TcpConnection(int fd);  //传进去fd，用来初始化SockIO
    ~TcpConnection();
    void send(const string &msg);
    string receive();
    string toString();
    bool isClose() const;

    //三个回调函数注册
    void setConnectionCallback(const TcpConnectionCallback &cb); //连接建立
    void setMessageCallback(const TcpConnectionCallback &cb);    //消息到达
    void setCloseCallback(const TcpConnectionCallback &cb);  //连接断开

    //三个回调函数执行
    void handleConnectionCallback();   //处理连接建立
    void handleMessageCallback();   //处理消息发送
    void handleCloseCallback();     //处理连接断开
    

private:
    //获取本端地址与对端地址
    InetAddress getLocalAddr();
    InetAddress getPeerAddr();

private:
    SocketIO _sockIO;

    Socket _sock;
    //本端地址与对端地址
    InetAddress _localAddr;
    InetAddress _peerAddr;

    TcpConnectionCallback _onConnectionCb;
    TcpConnectionCallback _onMessageCb;
    TcpConnectionCallback _onCloseCb;
};

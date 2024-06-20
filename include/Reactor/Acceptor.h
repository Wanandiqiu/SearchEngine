#pragma once
#include"Socket.h"
#include"InetAddress.h"
#include<string>

using std::string;

class Acceptor
{
public:
    Acceptor(const string &ip, unsigned short port);
    ~Acceptor();
    void ready();
    void setReuseAddr();
    void setReusePort();
    void bind();
    void listen();
    int accept();

    int fd();   // 返回监听的fd

private:
    Socket _listenSock;
    InetAddress _servAddr;
};


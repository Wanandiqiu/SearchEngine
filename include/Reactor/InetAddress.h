#pragma once
#include<arpa/inet.h>
#include<string>

using std::string;

class InetAddress
{
public:
    InetAddress(const string &ip, unsigned short port); //通过IP与端口初始化
    InetAddress(const struct sockaddr_in &addr);        //通过结构体初始化
    ~InetAddress();
    string ip() const;
    unsigned short port() const;
    const struct sockaddr_in *getInetAddrPtr() const;

private:
    struct sockaddr_in _addr;
};


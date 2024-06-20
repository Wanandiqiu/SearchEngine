#include"InetAddress.h"
#include<strings.h>

InetAddress::InetAddress(const string &ip, unsigned short port){

    ::bzero(&_addr, sizeof(struct sockaddr_in));    //bzero将指定内存块的前n个字节全部设置为零
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);
    _addr.sin_addr.s_addr = inet_addr(ip.c_str());

}

InetAddress::InetAddress(const struct sockaddr_in &addr):_addr(addr){

}

InetAddress::~InetAddress(){

}

string InetAddress::ip() const{ //获取服务器IP

    //网络字节序 -> 本机字节序
    return string(inet_ntoa(_addr.sin_addr));

}

unsigned short InetAddress::port() const{   //获取服务器端口

    return ntohs(_addr.sin_port);
}


const struct sockaddr_in* InetAddress::getInetAddrPtr() const{
    return &_addr;
}


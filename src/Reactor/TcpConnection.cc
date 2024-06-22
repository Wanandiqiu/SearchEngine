#include"TcpConnection.h"
#include "EventLoop.h"
#include<iostream>
#include<sstream>

using std::cout;
using std::endl;
using std::ostringstream;

TcpConnection::TcpConnection(int fd):
_sockIO(fd)
,_sock(fd)  //fd就是调用accept建立好连接的fd，调用Socket有参构造初始化，用来后面获取到本端与对端的地址
,_localAddr(getLocalAddr())
,_peerAddr(getPeerAddr())
{
    
}

TcpConnection::~TcpConnection(){

}

void TcpConnection::send(const string &msg){
    _sockIO.writen(msg.c_str(), msg.size());
}

string TcpConnection::receive(){
    char buff[65535] = {0};
    _sockIO.readLine(buff, sizeof(buff));   //读一行数据，存到buff

    return string(buff);    //返回string
}

//_pegetpeernameerAddr用此函数完成初始化
InetAddress TcpConnection::getPeerAddr(){
    
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);
    //获取对端地址的函数getpeername
    int ret = getpeername(_sock.fd(), (struct sockaddr*)&addr, &len);
    if(-1 == ret){
        perror("getpeername");
    }
    return InetAddress(addr);
}

InetAddress TcpConnection::getLocalAddr(){
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);
    //获取本端地址的函数gesockname
    int ret = getsockname(_sock.fd(), (struct sockaddr*)&addr, &len);
    if(-1 == ret){
        perror("getsockname");
    }
    return InetAddress(addr);
}

string TcpConnection::toString(){

    ostringstream oss;
    
    oss << _localAddr.ip() << ":"
        << _localAddr.port() << "--->"
        << _peerAddr.ip() << ":"
        << _peerAddr.port();
    
    return oss.str();
}


//不能用右值做参数
void TcpConnection::setConnectionCallback(const TcpConnectionCallback &cb){
    _onConnectionCb = cb;
}

void TcpConnection::setMessageCallback(const TcpConnectionCallback &cb){
    _onMessageCb = cb;
}

void TcpConnection::setCloseCallback(const TcpConnectionCallback &cb){
    _onCloseCb = cb;
}

//三个回调函数执行
void TcpConnection::handleConnectionCallback(){
    if(_onConnectionCb){
        // function<void(const shared_ptr<TcpConnection> &)> _onConnectionCb;
        //_onConnectionCb是返回类型是void，参数是const TcpConnectionPtr &类型
        //使用shared_from_this()，避免智能指针的误用
        _onConnectionCb(shared_from_this());
    }
}

void TcpConnection::handleMessageCallback(){
    if(_onMessageCb){
        _onMessageCb(shared_from_this());
    }
}

void TcpConnection::handleCloseCallback(){
    if(_onCloseCb){
        _onCloseCb(shared_from_this());
    }
}


bool TcpConnection::isClose() const{
    
    char buf[10] = {0};
    int ret = ::recv(_sock.fd(), buf, sizeof(buf), MSG_PEEK); //只是进行读，没有将数据移走

    return (0 == ret);
}

// 在EventLoop中进行发送数据，将msg交给EventLoop，让其进行发送
void TcpConnection::sendInLoop(const string &msg){
    if(_loop){
        //让EventLoop去发送
        _loop->runInLoop(std::bind(&TcpConnection::send, this, msg));
    }
}
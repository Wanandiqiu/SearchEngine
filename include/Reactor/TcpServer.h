#pragma
#include "Acceptor.h"
#include "EventLoop.h"

/*
    继续封装，把 Acceptor 和 EventLoop 封装起来，对外只给用户提供 IP 和 port 的接口
*/

using TcpConnectionPtr = shared_ptr<TcpConnection>;
using TcpConnectionCallback = function<void(const TcpConnectionPtr &con)>; 

class TcpServer
{
public:
    TcpServer(const string &ip, unsigned short port);
    ~TcpServer();

    void start();
    void stop();

    void setAllCallback(TcpConnectionCallback &&onConnection
                        , TcpConnectionCallback &&onMessage
                        , TcpConnectionCallback &&onClose);
private:
    Acceptor _acceptor;
    EventLoop _loop;
};
#include "EventLoop.h"

EventLoop::EventLoop(Acceptor &acceptor):
_epfd(CreateEpollFd()),
_acceptor(acceptor),
_isLooping(false),
_evList(1024)
{
    // 把监听的文件描述符 listenfd 放到红黑树上
    AddEpollFd(_acceptor.fd());
}


EventLoop::~EventLoop()
{
    close(_epfd);
}

//回调函数注册(中转)
void EventLoop::setConnectCallback(TcpConnectionCallback &&cb){
    _onConnectionCb = std::move(cb);
}


void EventLoop::setMessageCallback(TcpConnectionCallback &&cb){

    _onMessageCb = std::move(cb);
}


void EventLoop::setCloseCallback(TcpConnectionCallback &&cb){

    _onCloseCb = std::move(cb);
}

void EventLoop::loop()
{
    _isLooping = true;

    while(_isLooping)
    {
        WaitEpollFd();
    }
}


void EventLoop::unloop()
{
    _isLooping = false;
}

int EventLoop::CreateEpollFd()
{
    int fd = epoll_create1(0);
    if(fd < 0)
    {
        perror("epoll_create");
        return -1;
    }
    return fd;
}

void EventLoop::WaitEpollFd()
{
    int nready;
    do{
        // epol_wait中第二个参数是一个结构体指针，可获取vector第一个元素首地址，也可以用 _evList.data()
        nready = epoll_wait(_epfd, _evList.data(), _evList.size(), 5000);   // epoll_wait 成功时返回准备好的文件描述符的数量
    }while(-1 == nready && errno == EINTR);

    if(-1 == nready)
    {
        std::cout << "epoll_wait error" << std::endl;
        return;
    }
    else if(0 == nready)
    {
        std::cout << "epoll_wait timeout" << std::endl;
    }
    else
    {
        //考虑到扩容的问题，因为初始情况下监听的文件描述符只写了1024
        if(nready == (int)_evList.size()){
            _evList.resize(2 * nready);
        }

        // 正常情况
        for(size_t idx = 0; idx < nready; idx++)
        {
            int fd = _evList[idx].data.fd;
            // 是监听的listenfd，有新的连接请求进来
            if(fd == _acceptor.fd())
            {
                if(_evList[idx].events == EPOLLIN)
                {
                    // 处理新的连接请求
                    handleNewConnection();
                }
            }
            else
            {
                // 是老的连接，处理老的连接上发来的数据
                if(_evList[idx].events == EPOLLIN)
                {
                    handleMessage(fd);
                }
            }
        }
    }
}
    

void EventLoop::AddEpollFd(int fd)
{   
    struct epoll_event evt;
    evt.events = EPOLLIN;
    evt.data.fd = fd;

    int ret = ::epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &evt);
    if(ret < 0){
        perror("epoll_ctl add\n");
        return;
    }
}

void EventLoop::DelEpollFd(int fd)
{
    struct epoll_event evt;
    evt.events = EPOLLIN;
    evt.data.fd = fd;

    int ret = ::epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, &evt);
    if(ret < 0){
        perror("epoll_ctl delete\n");
        return;
    }
}


void EventLoop::handleNewConnection()
{
    // 处理新的连接请求（accept 连接，创建一条 TcpConnection，就能收发数据了，要把新建立连接的fd放到红黑树上监听读写事件
    int connfd = _acceptor.accept();
    if(connfd < 0){
        perror("handleNewConnection\n");
        return;
    }
    //将新的文件描述符放到红黑树监听
    AddEpollFd(connfd);

    //如果connfd是正常的，那么就表明三次握手成功，表明连接建立好了
    TcpConnectionPtr con(new TcpConnection(connfd));    //map中value是智能指针

    // 注册三个回调函数
    con->setConnectionCallback(_onConnectionCb);    //连接建立
    con->setMessageCallback(_onMessageCb);     //消息到达
    con->setCloseCallback(_onCloseCb);      //连接断开

    _conns.insert({connfd, con});

    //满足连接条件建立，执行回调函数
    con->handleConnectionCallback();     //处理连接建立
}

void EventLoop::handleMessage(int fd){

    auto it = _conns.find(fd);
    if(it != _conns.end()){
        bool flag = it->second->isClose();
        //如果连接断开
        if(flag){
            //处理连接断开
            it->second->handleCloseCallback();
            //fd从红黑树删除
            DelEpollFd(fd); 
            //将connfd与连接这个键值对从map删除
            _conns.erase(it);
        }else{
            //如果连接没有断开
            it->second->handleMessageCallback();    //处理消息发送
        }

    }else{
        std::cout << "连接不存在" << std::endl;
        return;
    }
}


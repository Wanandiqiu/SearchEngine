#pragma once
#include"NoCopyable.h"

class Socket
:public NonCopyable{
public:
    Socket();
    explicit Socket(int fd);
    ~Socket();
    int fd() const;
    void shutDownWrite();

private:
    int _fd;
};


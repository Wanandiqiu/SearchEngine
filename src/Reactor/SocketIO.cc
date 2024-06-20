#include "SocketIO.h"
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

SocketIO::SocketIO(int fd) : _fd(fd)
{
}

SocketIO::~SocketIO()
{
    close(_fd);
}

// len = 10000
int SocketIO::readn(char *buf, int len)
{
    int left = len;
    char *pstr = buf; // 读取到的数据存储到pstr所指向的内存中
    int ret = 0;

    while (left > 0)
    {
        ret = read(_fd, pstr, left); // read返回实际读取到的字节数
        if (-1 == ret && errno == EINTR)
        {
            continue;
        }
        else if (-1 == ret)
        {
            perror("read error");
            return len - ret;
        }
        else
        {
            pstr += ret;
            left -= ret;
        }
    }
    return len - left;
}

int SocketIO::writen(const char *buf, int len)
{
    // buf与left控制每次读取的大小
    int left = len;
    const char *pstr = buf;
    int ret = 0; // 每次读多少字节

    while (left > 0)
    {
        ret = write(_fd, pstr, left); // 写入pstr指向的内存中
        if (-1 == left && errno == EINTR)
        {
            continue;
        }
        else if (-1 == ret)
        {
            perror("write error");
            return len - left;
        }
        else
        {
            left -= ret;
            pstr += ret;
        }
    }
    return len - left;
}

int SocketIO::readLine(char *buf, int len)
{
    int left = len;
    char *pstr = buf;
    int ret = 0, total = 0;

    while (left > 0)
    {
        // MSG_PEEK不会将缓冲区中的数据进行清空，只会进行拷贝操作
        ret = recv(_fd, pstr, left, MSG_PEEK);
        if (-1 == ret && errno == EINTR)
        {
            continue;
        }
        else if (-1 == ret)
        {
            perror("readLine error");
            return len - left;
        }
        else if (0 == ret)
        {
            break;
        }
        else
        {
            for (int idx = 0; idx < ret; idx++)
            {
                if (pstr[idx] == '\n')
                {
                    // 获取到\n之前的字符，将其读入
                    int sz = idx + 1;
                    readn(pstr, sz);
                    pstr += sz;
                    *pstr = '\0';

                    return total + sz;
                }
            }

            readn(pstr, ret); // 没有读取到\n，从内核态拷贝到用户态
            total += ret;
            left -= ret;
            pstr += ret;
        }
    }
    *pstr = '\0';

    return total - left;
}

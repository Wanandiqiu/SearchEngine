#include <iostream>
#include "EchoServer.h"

int main()
{   
  
    EchoServer server(4, 10, "127.0.0.1", 8888);
    server.start();

    return 0;
}


#include <iostream>

#include"httpServer.h"
using namespace std;

int main(int argc,char *argv[])
{
    const char *ip="0£¬0£¬0£¬0";
    int port=8080;
    httpServer* Server = new httpServer(port, ip, 8, 0);
    Server->event_listen();
    Server->event_loop();
    return 0;
}

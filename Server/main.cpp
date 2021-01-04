#include <QCoreApplication>
#include "communicatorserver.h"
#include <iostream>
#include <windows.h>
#include <QDebug>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    CommunicatorServer* server = CommunicatorServer::getInstance();
    int port;
    std::cout << "Port to listen: ";
    std::cin >> port;
    server->run(port);

    return a.exec();
}

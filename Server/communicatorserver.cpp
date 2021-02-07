#include "communicatorserver.h"

CommunicatorServer::CommunicatorServer(QObject* parent)
    : QTcpServer(parent)
{}

CommunicatorServer::~CommunicatorServer()
{}

CommunicatorServer* CommunicatorServer::m_instance = nullptr;
CommunicatorServer* CommunicatorServer::getInstance(QObject *parent)
{
    if(m_instance == nullptr)
    {
        m_instance = new CommunicatorServer(parent);
    }
    return m_instance;
}

void CommunicatorServer::incomingConnection(qintptr handle)
{
   ClientControler* controler = ClientControler::instance();
   Client* client = new Client(handle);
   connect(client, &Client::finished, client, &Client::deleteLater);
   connect(client, &Client::destroyed, this, [&]()
		   {
		   	std::cout << "Klient usuniety " << std::endl;
		   });
   std::cout <<"Polaczenie klienta z handlem : " << handle << std::endl;
   controler->addClient(client);
   client->start();
}

void CommunicatorServer::run(int port)
{
    this->listen(QHostAddress::Any, port);
    qDebug() << "listening for connection port:" << port;
}

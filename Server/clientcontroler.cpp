#include "clientcontroler.h"
#include <iostream>

ClientControler* ClientControler::m_instance = nullptr;

ClientControler::ClientControler()
{}
ClientControler::~ClientControler()
{}

ClientControler* ClientControler::instance()
{
    if(m_instance == nullptr)
    {
        m_instance = new ClientControler;
    }
    return m_instance;
}

void ClientControler::addClient(Client *client)
{
    connect(client, &Client::disconnect, this, &ClientControler::clientDisconnect);
    m_clients.addElement(client);
}

void ClientControler::clientDisconnect()
{
    m_locker.lock();
    m_clients.removeDisconnectedClient();
    m_locker.unlock();
}

void ClientControler::sendToAll(char* data, size_t size, bool skip, qintptr handle)
{
         Client *c;
        for(unsigned int i = 0; i < m_clients.size(); i++)
        {
            c = m_clients.element(i);
            if(skip == true && handle == m_clients.element(i)->handle())
                continue;
            c->addToQueue(QByteArray(data, size));
        }
}

bool ClientControler::sendPrivate(const char* data, size_t size, char* target)
{
    Client* c;
    for(unsigned int i = 0; i < m_clients.size(); i++)
    {
        c = m_clients.element(i);
        if(strcmp(target, c->name()) == 0)
        {
            c->addToQueue(QByteArray(data, size));
            return true;
        }
    }
    return false;
}

void ClientControler::sendMsg(const char *data, size_t size, char **targets, unsigned int targetsAmount)
{
    for(unsigned int i = 0; i < targetsAmount; i++)
    {
        sendPrivate(data, size, targets[i]);
    }
}

void ClientControler::createUserList(Client *client)
{
    Client* c;
    MainHeader header;
    header.size = (m_clients.size() - 1) * USER_NAME_SIZE;
    header.version = PROTOCOL_VERSION;
    header.typeMessage = MessageType::Connected;
    client->addToQueue(QByteArray((char*)&header, sizeof(MainHeader)));
    for(unsigned int i = 0; i < m_clients.size(); i++)
    {
        c = m_clients.element(i);
        if(client->handle() != c->handle())
        {
            client->addToQueue(QByteArray(c->name(), USER_NAME_SIZE));
        }
    }
}


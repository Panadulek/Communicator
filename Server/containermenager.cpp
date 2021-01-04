#include "containermenager.h"

ContainerMenager::ContainerMenager()
{}

ContainerMenager::~ContainerMenager()
{}

void ContainerMenager::addElement(Client *socket)
{
    m_locker.lock();

    m_clients.push_back(socket);
    m_locker.unlock();
}
Client* ContainerMenager::element(int idx)
{
    Client* ret;
    m_locker.lock();
    if(idx >= m_clients.size())
        ret = nullptr;
    else
        ret = m_clients[idx];
    m_locker.unlock();
    return ret;

}

Client* ContainerMenager::elementDescription(qintptr desc)
{
    Client *ret = nullptr;
    m_locker.lock();
    for(int i = 0; i < m_clients.size(); i++)
    {
        if(m_clients[i]->handle() == desc)
        {
            ret = m_clients[i];
            break;
        }
    }
    m_locker.unlock();
    return ret;
}

Client* ContainerMenager::last()
{
    Client *ret;
    m_locker.lock();
    ret = m_clients.last();
    m_locker.unlock();
    return ret;
}




void ContainerMenager::removeDisconnectedClient()
{
    m_locker.lock();
    Client* c;
    for(int i = 0; i < m_clients.size(); i++)
    {

        if(m_clients[i]->handle() == -1){

            c = m_clients[i];
            m_clients.remove(i);
            delete c;
        }
    }
    m_locker.unlock();
}

size_t ContainerMenager::size()
{
    size_t size;
    m_locker.lock();
    size = m_clients.size();
    m_locker.unlock();
    return size;
}


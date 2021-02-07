#include "client.h"
#include "clientcontroler.h"
Client::Client(qintptr handle)
    :QThread(), m_socket(nullptr), m_handle(handle)
{
    m_name[0] = '\0';
}

Client::~Client()
{
}
qintptr Client::handle()
{
    return m_socket->socketDescriptor();
}

const char* Client::name()
{
    return m_name;
}


bool Client::waitForData(size_t size)
{
    bool ok = false;
    do
    {
        size_t sizeAvailable = m_socket->bytesAvailable();
        {
            if (sizeAvailable >= size)
            {
                std::cout << "dostepne dane: " << sizeAvailable << std::endl;
                ok = true;
                break;
            }
        }
    }while (m_socket->waitForReadyRead(100));
    return ok;
}

void Client::addToQueue(const QByteArray &data)
{
    m_locker.lock();
    m_queue.enqueue(data);
    m_locker.unlock();
}

void Client::run()
{
    m_socket = new QTcpSocket;
    m_socket->setSocketDescriptor(m_handle);
    MainHeader mainHeader;
    if(m_socket->state() == QTcpSocket::ConnectedState)
        ClientControler::instance()->createUserList(this);
    while(m_socket->state() == QTcpSocket::ConnectedState)
    {
        m_locker.lock();
        while (m_queue.size() > 0)
        {
            QByteArray data = m_queue.dequeue();
            std::string str = data.toStdString();
            m_socket->write(str.c_str(), data.size());
            m_socket->waitForBytesWritten(100);
            m_socket->flush();

        }
        m_locker.unlock();
        if (waitForData(sizeof(MainHeader)))
        {
            m_socket->read((char*)&mainHeader, sizeof(MainHeader));
            if(mainHeader.typeMessage == MessageType::NewConnection)
            {
                m_socket->read(m_name, USER_NAME_SIZE);
                m_name[USER_NAME_SIZE - 1] = '\0';
                m_valid = true;
                std::cout << "Wyslanie wszystkim mojego nicku: " << m_handle << std::endl;
                ClientControler::instance()->sendToAll((char*)&mainHeader, sizeof(MainHeader), false, m_handle);
                ClientControler::instance()->sendToAll(m_name, USER_NAME_SIZE, false, m_handle);
            }
            else if(mainHeader.typeMessage == MessageType::SimpleMessageType)
            {
                if(waitForData(mainHeader.size))
                {
                    mainHeader.size += USER_NAME_SIZE;
                    ClientControler::instance()->sendToAll((char*)&mainHeader, sizeof(MainHeader));
                    ClientControler::instance()->sendToAll(m_name, USER_NAME_SIZE);
                    mainHeader.size -= USER_NAME_SIZE;
                    char *message = new char[mainHeader.size + 1];
                    m_socket->read(message, mainHeader.size);
                    message[mainHeader.size] = '\0';
                    ClientControler::instance()->sendToAll(message, mainHeader.size);
                    delete[] message;
                }
            }
            else if(mainHeader.typeMessage == MessageType::PrivateMessageType)
            {
                char targetName[USER_NAME_SIZE];
                constexpr int amountTargets = 2;
                char *message = nullptr;
                if(waitForData(mainHeader.size))
                {
                    m_socket->read(targetName, USER_NAME_SIZE);
                    targetName[USER_NAME_SIZE - 1] = '\0';
                    mainHeader.size -= USER_NAME_SIZE;
                    message = new char[mainHeader.size + 1];
                    m_socket->read(message, mainHeader.size);
                    message[mainHeader.size] = '\0';
                }
                char *targets[amountTargets];
                targets[0] = targetName;
                targets[1] = m_name;
                mainHeader.size += USER_NAME_SIZE;
                mainHeader.size += USER_NAME_SIZE;
                ClientControler::instance()->sendPrivate((char*)&mainHeader, sizeof(MainHeader), m_name);
                ClientControler::instance()->sendPrivate((char*)&mainHeader, sizeof(MainHeader), targetName);
                ClientControler::instance()->sendPrivate(targetName, USER_NAME_SIZE, m_name);
                ClientControler::instance()->sendPrivate(targetName, USER_NAME_SIZE, targetName);
                ClientControler::instance()->sendPrivate(m_name, USER_NAME_SIZE, m_name);
                ClientControler::instance()->sendPrivate(m_name, USER_NAME_SIZE, targetName);
                mainHeader.size -= USER_NAME_SIZE;
                mainHeader.size -= USER_NAME_SIZE;
                ClientControler::instance()->sendPrivate(message, mainHeader.size, m_name);
                ClientControler::instance()->sendPrivate(message, mainHeader.size, targetName);
                if(message != nullptr)
                    delete [] message;
            }

            else if(mainHeader.typeMessage == MessageType::FileRequest)
            {
                char target[USER_NAME_SIZE];
                m_socket->read(target, mainHeader.size);
                ClientControler::instance()->sendPrivate((char*)&mainHeader, sizeof(mainHeader), target);
                ClientControler::instance()->sendPrivate(m_name, mainHeader.size, target);
            }
            else if(mainHeader.typeMessage == MessageType::FileRequestAccepted)
            {
                char target[USER_NAME_SIZE];
                char *port = new char[mainHeader.size - USER_NAME_SIZE];
                char ip[IP_SIZE];
                strcpy(ip, m_socket->peerAddress().toString().toStdString().c_str());
                ip[strlen(ip) + 1] = '\0';
                m_socket->read(target, USER_NAME_SIZE);
                mainHeader.size -= USER_NAME_SIZE;
                m_socket->read(port, mainHeader.size);
                mainHeader.size += IP_SIZE;
                ClientControler::instance()->sendPrivate((char*)&mainHeader, sizeof(MainHeader), target);
                ClientControler::instance()->sendPrivate(ip, IP_SIZE, target);
                mainHeader.size -= IP_SIZE;
                ClientControler::instance()->sendPrivate(port, mainHeader.size, target);
                delete [] port;
            }
        }
    }
    
    mainHeader.size = USER_NAME_SIZE;
    mainHeader.typeMessage = MessageType::Disconnected;
    mainHeader.version = PROTOCOL_VERSION;
    ClientControler::instance()->sendToAll((char*)&mainHeader, sizeof(MainHeader), true, m_handle);
    ClientControler::instance()->sendToAll(m_name, mainHeader.size, true, m_handle);
    std::cout << "Rozlaczenie uzytkownika o handle'u: " << m_handle << std::endl;
    m_socket->close();
    m_socket->deleteLater();
    m_socket = nullptr;
    emit disconnect(this);
}



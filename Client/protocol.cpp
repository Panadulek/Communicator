#include "protocol.h"

Protocol::Protocol(QObject *parent) : QObject(parent)
{
    m_socket = new QTcpSocket(this);
    connect(m_socket, &QTcpSocket::readyRead, this, &Protocol::read);
}

Protocol::~Protocol()
{
    if(m_socket->isOpen())
        m_socket->close();
}

bool Protocol::waitForData(qint64 size)
{
    bool ok = false;
    do
    {
        qint64 sizeAvailable = m_socket->bytesAvailable();
        if (sizeAvailable >= size)
        {
            ok = true;
            break;
        }
    } while (true);
    return ok;
}
void Protocol::connectToHost(const QString &ip, const QString &port)
{
    quint16 p = port.toUShort();
    m_socket->connectToHost(ip, p);
}
void Protocol::readData(size_t sizeToRead, QString &message)
{
    while(sizeToRead > 0)
    {
        if(sizeToRead >= PROTOCOL_MAX_BUFFER)
        {
            m_socket->read(m_messageBuffer, PROTOCOL_MAX_BUFFER);
            sizeToRead -= PROTOCOL_MAX_BUFFER;
            message += m_messageBuffer;
        }
        else
        {
            m_socket->read(m_messageBuffer, sizeToRead);
            sizeToRead -= sizeToRead;
            message += m_messageBuffer;
        }
    }

}
void Protocol::read()
{
    QString data;
    MainHeader mainHeader;
    if(waitForData(sizeof(mainHeader)))
    {
        m_socket->read(reinterpret_cast<char*>(&mainHeader), sizeof(mainHeader));
        if(mainHeader.typeMessage == MessageType::SimpleMessageType)
        {
            if(waitForData(mainHeader.size))
            {
                m_socket->read(m_nameBuffer, USER_NAME_SIZE);
                mainHeader.size -= USER_NAME_SIZE;
                readData(mainHeader.size, data);
                emit globalMessage(data, m_nameBuffer);
                m_nameBuffer[0] = '\0';
                data.clear();
            }
        }

        else if(mainHeader.typeMessage == MessageType::Connected)
        {
            if(waitForData(mainHeader.size))
            {
                while(mainHeader.size != 0)
                {
                    m_socket->read(m_nameBuffer, USER_NAME_SIZE);
                    mainHeader.size -= USER_NAME_SIZE;
                    emit userJoined(QString(m_nameBuffer));
                }
            }
        }
        else if(mainHeader.typeMessage == MessageType::NewConnection)
        {
            if(waitForData(mainHeader.size))
            {
                m_socket->read(m_nameBuffer, USER_NAME_SIZE);
                mainHeader.size -= USER_NAME_SIZE;
                emit userJoined(QString(m_nameBuffer));
            }
        }
        else if(mainHeader.typeMessage == MessageType::Disconnected)
        {
            if(waitForData(mainHeader.size))
            {
                m_socket->read(m_nameBuffer, USER_NAME_SIZE);
                mainHeader.size -= USER_NAME_SIZE;
                emit userDisconnected(m_nameBuffer);
            }
        }
    }

}

void Protocol::sendNicknameMessage(const QString &nickname)
{
    MainHeader mainHeader;
    mainHeader.size = 0;
    mainHeader.typeMessage = MessageType::NewConnection;
    mainHeader.size += USER_NAME_SIZE;
    mainHeader.version = PROTOCOL_VERSION;
    m_socket->write(reinterpret_cast<char*>(&mainHeader), sizeof(MainHeader));
    m_socket->write(nickname.toStdString().c_str(), mainHeader.size);
}

void Protocol::sendGlobalMessage(const QString &message)
{
    MainHeader mainHeader;
    mainHeader.size = 0;
    mainHeader.typeMessage = MessageType::SimpleMessageType;
    mainHeader.size += message.size();
    mainHeader.version = PROTOCOL_VERSION;
    QString messageParsed = "";
    m_socket->write(reinterpret_cast<char*>(&mainHeader), sizeof(MainHeader));
    do
    {
        if(mainHeader.size > PROTOCOL_MAX_BUFFER)
        {

        }
        else
        {
            m_socket->write(message.toStdString().c_str(), mainHeader.size);
        }
    }while(mainHeader.size <= 0);
}

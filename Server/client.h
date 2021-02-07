#ifndef CLIENT_H
#define CLIENT_H
#include <QTcpSocket>
#include <cstring>
#include "../protocolheaders.h"
#include <QThread>
#include <QMutex>
#include <QQueue>
#include <QHostAddress>
#include <iostream>
class Client : public QThread
{
Q_OBJECT
private:

    QTcpSocket* m_socket;
    QMutex m_locker;
    char m_name[USER_NAME_SIZE];
    qintptr m_handle;
    bool m_valid = false;
    QQueue<QByteArray> m_queue;
    void run() override;

    bool waitForData(size_t size);
signals:
    void disconnect(Client *);
public:
    Client(qintptr handle);
    ~Client();
    qintptr handle();
    bool isValid(){return m_valid;}
    const char* name();
    void addToQueue(const QByteArray &data);
};

#endif // CLIENT_H

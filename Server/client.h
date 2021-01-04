#ifndef CLIENT_H
#define CLIENT_H
#include <QTcpSocket>
#include <cstring>
#include "../protocolheaders.h"
#include <QThread>
#include <QMutex>
#include <QQueue>
#include "../Common/networkstring.h"
#include <QHostAddress>
class Client : public QThread
{
Q_OBJECT
private:

    QTcpSocket* m_socket;
    QMutex m_locker;
    char m_name[USER_NAME_SIZE];
    qintptr m_handle;
    QQueue<QByteArray> m_queue;
    void run() override;

    bool waitForData(size_t size);
signals:
    void disconnect();
public:
    Client(qintptr handle);
    ~Client();
    qintptr handle();
    const char* name();
    void addToQueue(const QByteArray &data);
};

#endif // CLIENT_H

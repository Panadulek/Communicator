#ifndef COMMUNICATORSERVER_H
#define COMMUNICATORSERVER_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include <QDebug>
#include <QMutex>
#include <QByteArray>
#include "clientcontroler.h"
#include <iostream>
class CommunicatorServer : public QTcpServer
{
    Q_OBJECT
private:
    static CommunicatorServer* m_instance;
    QMutex m_locker;
    void incomingConnection(qintptr handle) override;
    CommunicatorServer(QObject* parent = nullptr);


public slots:

public:
    virtual ~CommunicatorServer();
    static CommunicatorServer* getInstance(QObject* parent = nullptr);

    void run(int port);

};


#endif // COMMUNICATORSERVER_H

#ifndef CLIENTCONTROLER_H
#define CLIENTCONTROLER_H
#include <QTcpSocket>
#include <QByteArray>
#include <QString>
#include <QMutex>
#include <containermenager.h>
#include "client.h"
#include "../protocolheaders.h"
#include <string>
class ClientControler : public QObject
{
Q_OBJECT
private:
    ContainerMenager m_clients;
    QMutex m_locker;
    static ClientControler* m_instance;

protected:
    ClientControler();
signals:
    void disconnect(Client *c);
public slots:
    void clientDisconnect(Client *);
public:
    static ClientControler* instance();
    virtual ~ClientControler();
    void addClient(Client* client);
    void sendMsg(const char* data, size_t size, char** targets, unsigned targetsAmount);
    void sendToAll(char* data, size_t size, bool skip = false, qintptr handle = -1);
    bool sendPrivate(const char* data, size_t size, char* target);
    Client* getTarget(const char* name);
    void createUserList(Client* client);
    void refillClientInfo();
};

#endif // CLIENTCONTROLER_H

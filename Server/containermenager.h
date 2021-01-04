#ifndef CONTAINERMENAGER_H
#define CONTAINERMENAGER_H
#include <QThread>
#include <QMutex>
#include <QTcpSocket>
#include <QVector>
#include "client.h"
class ContainerMenager : public QObject
{
Q_OBJECT
private:
    QVector<Client*> m_clients;
    QMutex m_locker;
protected:

public:
    ContainerMenager();
    virtual ~ContainerMenager();
    void addElement(Client* socket);
    Client* element(int idx);
    Client* elementDescription(qintptr desc);
    Client* last();
    void removeDisconnectedClient();
    size_t size();

};

#endif // CONTAINERMENAGER_H

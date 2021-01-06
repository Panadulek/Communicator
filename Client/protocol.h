#ifndef PROTOCOL_H
#define PROTOCOL_H
#include <QObject>
#include <QTcpSocket>
#include <QString>
#include "../protocolheaders.h"

class Protocol : public QObject
{
    Q_OBJECT
    QTcpSocket *m_socket;
    char m_nameBuffer[USER_NAME_SIZE];
    char m_messageBuffer[PROTOCOL_MAX_BUFFER];
    bool waitForData(qint64 size);
    void readData(size_t sizeToRead, QString &message);
public:
    explicit Protocol(QObject *parent = nullptr);
    ~Protocol();
    const QTcpSocket* socket(){return m_socket;}
    void connectToHost(const QString &, const QString &);
    void disconnectFromHost();
private slots:
    void read();
public slots:
    void sendNicknameMessage(const QString &);
    void sendGlobalMessage(const QString &);
signals:
    void globalMessage(QString &message, QString &user);
    void userJoined(const QString &nickname);
    void userDisconnected(const QString &nickname);
};

#endif // PROTOCOL_H

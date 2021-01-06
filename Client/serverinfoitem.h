#ifndef SERVERINFOITEM_H
#define SERVERINFOITEM_H

#include <QObject>
#include <QListWidgetItem>
#include <QListWidget>
#include <QString>
#define SERVER_INFO_ITEM 0


class ServerInfoItem : public QListWidgetItem
{
    bool m_validNickname = false;
    QString m_message = "";
    QString m_myNickname = "";
    explicit ServerInfoItem(QListWidget *parent = nullptr);
    static ServerInfoItem* s_instance;
public:
    static ServerInfoItem* instance();
    bool isNickNameValid(){return m_validNickname;}
    void setNicknameValid(bool ok){m_validNickname = ok;}
    void setNickname(const QString &nickname){m_myNickname = nickname;}
    const QString &myNickname(){return m_myNickname;}
    QString message() {return m_message;}
    void setMessage(const QString &msg)
    {
        m_message = msg;
    }
    void appendMessage(const QString &msg)
    {
        m_message.append(msg);
    }
    bool operator<(const QListWidgetItem &other) const override
    {
        return false;
    }
signals:

};

#endif // SERVERINFOITEM_H

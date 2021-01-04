#ifndef SERVERINFOITEM_H
#define SERVERINFOITEM_H

#include <QObject>
#include <QListWidgetItem>
#include <QListWidget>
#include <QString>
class ServerInfoItem : public QListWidgetItem
{
    bool m_validNickname = false;
    QString m_message = "";
public:
    explicit ServerInfoItem(QListWidget *parent = nullptr);
    bool isNickNameValid(){return m_validNickname;}
    void setNicknameValid(bool ok){m_validNickname = ok;}
    QString message() {return m_message;}
    void setMessage(const QString &msg)
    {
        m_message = msg;
    }
    void appendMessage(const QString &msg)
    {
        m_message.append(msg);
    }
signals:

};

#endif // SERVERINFOITEM_H

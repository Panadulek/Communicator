#ifndef USER_ITEM_H
#define USER_ITEM_H
#include <QListWidgetItem>
#include <QString>

#define GLOBAL_ITEM 1
#define MY_ITEM 2

class UserItem : public QListWidgetItem
{
private:
	QString m_message = "";
    bool m_isGlobal = false;
public:
	UserItem(const QString &title, QListWidget *parent = nullptr);
	QString message() {return m_message;}
    void setMessage(const QString &msg)
	{
		m_message = msg;
	}	
	void appendMessage(const QString &msg)
	{
        m_message.append(msg);
	}
    void setGlobalChat(bool ok = true){m_isGlobal = ok;}
    bool isGlobalChat() const {return m_isGlobal;}
    bool operator<(const QListWidgetItem &other) const override;
};

#endif

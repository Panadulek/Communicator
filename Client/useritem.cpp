#include "useritem.h"
#include "serverinfoitem.h"
#include <QDebug>
UserItem::UserItem(const QString &title, QListWidget *parent) 
    :QListWidgetItem(title, parent)
{}

bool UserItem::operator<(const QListWidgetItem &other) const
{

    if(reinterpret_cast<QListWidgetItem*>(const_cast<UserItem*>(this)) == reinterpret_cast<QListWidgetItem*>(ServerInfoItem::instance()))
        return false;
    else if(this->isGlobalChat())
        return false;
    else if(reinterpret_cast<UserItem*>(const_cast<QListWidgetItem*>(&other))->isGlobalChat())
        return true;
    else if(other.text() == ServerInfoItem::instance()->myNickname())
        return true;
    else if(this->text() == ServerInfoItem::instance()->myNickname())
        return false;
    return QListWidgetItem::operator<(other);
}

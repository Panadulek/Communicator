#include "serverinfoitem.h"

ServerInfoItem::ServerInfoItem(QListWidget *parent) : QListWidgetItem("ServerInfo", parent)
{
    m_message = "Disconnected";
}

#include "serverinfoitem.h"
ServerInfoItem* ServerInfoItem::s_instance = nullptr;

ServerInfoItem* ServerInfoItem::instance()
{
    if(s_instance == nullptr)
        s_instance = new ServerInfoItem(nullptr);
    return s_instance;
}

ServerInfoItem::ServerInfoItem(QListWidget *parent) : QListWidgetItem("ServerInfo", parent)
{
    m_message = "Disconnected";
}

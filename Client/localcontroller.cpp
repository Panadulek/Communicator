#include "localcontroller.h"
#include <QListWidget>
#include <QTextEdit>
#include <QTabBar>
#include <QScrollBar>
LocalController::LocalController(QObject *parent, MainWindow *mw, Protocol *p) : QObject(parent), m_mainWindow(mw), m_protocol(p)
{
    m_msgBox.addButton(QMessageBox::StandardButton::Ok);
    initGuiController();
    connect(&m_dialog, &QDialog::accepted, this, &LocalController::provideConnectionData);
    connect(m_protocol->socket(), &QTcpSocket::connected, this, &LocalController::updateConnectionInfo);
    connect(m_protocol, &Protocol::userJoined, this, &LocalController::addUser);
    connect(m_protocol, &Protocol::userDisconnected, this, &LocalController::removeUser);
    connect(this, &LocalController::nicknameSetted, m_protocol, &Protocol::sendNicknameMessage);
    connect(this, &LocalController::newMessage, this, &LocalController::signalNewMessage);
    connect(m_mainWindow->tabBar(), &QTabBar::currentChanged, this, &LocalController::readChat);
}

void LocalController::initGuiController()
{
    connect(m_mainWindow->connectAction()->menu()->actions().at(0), &QAction::triggered, &m_dialog, &QDialog::exec);
    connect(m_mainWindow->connectAction()->menu()->actions().at(2), &QAction::triggered, this, &LocalController::disconnectFromHost);
    connect(m_mainWindow, &MainWindow::messageSent, this, &LocalController::sendMessage);
}

void LocalController::initChatMessages()
{
    connect(m_protocol, &Protocol::globalMessage, this, &LocalController::displayGlobalMessage);
    connect(m_protocol, &Protocol::privateMessage, this, &LocalController::displayPrivateMessage);
}
void LocalController::addUser(const QString &nickname)
{
    if(nickname.isEmpty())
        return;
    UserItem *item = new UserItem(nickname);
    m_mainWindow->listWidget()->addItem(item);
    if(nickname == m_serverInfoItem->myNickname())
    {
        QFont font;
        font.setWeight(QFont::Bold);
        item->setFont(font);
    }
    m_mainWindow->listWidget()->sortItems(Qt::SortOrder::DescendingOrder);
}
void LocalController::removeUser(const QString &nickname)
{
    UserItem *item = reinterpret_cast<UserItem*>(m_mainWindow->listWidget()->takeItem(findItemIndex(nickname)));
    m_mainWindow->listWidget()->removeItemWidget(item);
    if(item != nullptr)
        delete item;
    m_mainWindow->listWidget()->sortItems(Qt::SortOrder::DescendingOrder);
}

void LocalController::swapMsg(QListWidgetItem *item)
{
    QTextEdit *chat = m_mainWindow->chat();
    chat->setHtml(reinterpret_cast<UserItem*>(item)->message());
}
int LocalController::findTabIndex(const QString &text)
{
    QTabBar *tb = m_mainWindow->tabBar();
    for(int i = 0; i < tb->count(); i++)
        if(tb->tabText(i) == text)
            return i;
    return -1;
}
int LocalController::findItemIndex(const QString &text)
{
    QListWidget* lw = m_mainWindow->listWidget();
    for(int i = 0; i < lw->count(); i++)
        if(lw->item(i)->text() == text)
            return i;
    return -1;
}
void LocalController::addTab(QListWidgetItem *item)
{
    QTabBar *tabBar = m_mainWindow->tabBar();
    const QString &user = item->text();
    int index = findTabIndex(user);
    if(index >= 0)
    {
        tabBar->setCurrentIndex(index);
    }
    else
    {
        tabBar->addTab(user);
        tabBar->setCurrentIndex(tabBar->count()-1);
    }
}

void LocalController::changeTabData(int index)
{
    const QString &tabName = m_mainWindow->tabBar()->tabText(index);
    int itemIndex = findItemIndex(tabName);
    if(itemIndex >= 0)
        m_mainWindow->listWidget()->setCurrentRow(itemIndex);
}

void LocalController::closeTab(int index)
{
    if(index > 0)
    {
        changeTabData(index-1);
        m_mainWindow->tabBar()->removeTab(index);
    }
}
void LocalController::setupAppToChatting()
{
    connect(m_mainWindow->listWidget(), &QListWidget::currentItemChanged, this, &LocalController::swapMsg);
    connect(m_mainWindow->listWidget(), &QListWidget::currentItemChanged, this, &LocalController::addTab);
    connect(m_mainWindow->tabBar(), &QTabBar::currentChanged, this, &LocalController::changeTabData);
    connect(m_mainWindow->tabBar(), &QTabBar::tabCloseRequested, this, &LocalController::closeTab);
    m_serverInfoItem->setNicknameValid(true);
    m_mainWindow->listWidget()->setCurrentRow(GLOBAL_ITEM);
    initChatMessages();
}
void LocalController::sendMessage(const QString &message)
{
    if(m_protocol->socket()->state() != QAbstractSocket::ConnectedState || m_mainWindow->listWidget()->count() <= 0)
    {
        m_mainWindow->senderWidget()->lineEdit()->clear();
        return;
    }
    QListWidget *lw = m_mainWindow->listWidget();
    if(m_serverInfoItem != nullptr && m_serverInfoItem->isNickNameValid() == false)
    {
        if(findItemIndex(message) < 0)
        {
            emit nicknameSetted(message);
            m_serverInfoItem->setNickname(message);
            setupAppToChatting();
        }
        else
        {
            m_serverInfoItem->appendMessage("Nickname is reapeted");
        }
    }
    else if(lw->currentRow() == GLOBAL_ITEM)
    {
        m_protocol->sendGlobalMessage(message);
    }
    else if(lw->currentRow() >= MY_ITEM)
    {
        m_protocol->sendPrivateMessage(message, lw->currentItem()->text());
    }
    m_mainWindow->senderWidget()->lineEdit()->clear();
}

void LocalController::initServerInfoItem()
{
    if(m_serverInfoItem == nullptr)
    {
        m_serverInfoItem = ServerInfoItem::instance();
        m_mainWindow->listWidget()->addItem(m_serverInfoItem);
    }
    m_serverInfoItem->appendMessage("Connected to: " + m_protocol->socket()->peerName());
    m_serverInfoItem->appendMessage(NEW_LINE);
    m_mainWindow->listWidget()->setCurrentItem(m_serverInfoItem);
    addTab(m_serverInfoItem);
    m_serverInfoItem->appendMessage("A connection is established.");
    m_serverInfoItem->appendMessage(NEW_LINE);
    if(m_mainWindow->tabBar()->tabText(m_mainWindow->tabBar()->currentIndex()) == m_serverInfoItem->text())
        m_mainWindow->chat()->setText(m_serverInfoItem->message());

}

void LocalController::createGlobalChat()
{
    QString gc = "GLOBAL CHAT";
    addUser(gc);
    reinterpret_cast<UserItem*>(m_mainWindow->listWidget()->item(findItemIndex(gc)))->setGlobalChat(true);
}

void LocalController::disconnectFromHost()
{
    disconnect(m_mainWindow->listWidget(), &QListWidget::currentItemChanged, this, &LocalController::swapMsg);
    disconnect(m_mainWindow->listWidget(), &QListWidget::currentItemChanged, this, &LocalController::addTab);
    disconnect(m_mainWindow->tabBar(), &QTabBar::currentChanged, this, &LocalController::changeTabData);
    disconnect(m_mainWindow->tabBar(), &QTabBar::tabCloseRequested, this, &LocalController::closeTab);
    m_mainWindow->tabBar()->setCurrentIndex(0);
    for(int i = 0; i < m_mainWindow->tabBar()->count(); i++)
    {
        m_mainWindow->tabBar()->removeTab(i);
    }
    m_mainWindow->listWidget()->takeItem(0);
    m_mainWindow->listWidget()->clear();
    m_mainWindow->listWidget()->addItem(m_serverInfoItem);
    m_serverInfoItem->appendMessage("Disconnect from: ");
    m_serverInfoItem->appendMessage(m_protocol->socket()->peerName());
    m_serverInfoItem->appendMessage(NEW_LINE);
    m_mainWindow->chat()->setText(m_serverInfoItem->message());
    m_mainWindow->listWidget()->setCurrentItem(m_serverInfoItem);
    m_serverInfoItem->setNicknameValid(false);
    m_protocol->disconnectFromHost();
}

//------------------------Network------------------------------------//
//------------------------------------------------------------------//

void LocalController::provideConnectionData()
{
    m_protocol->connectToHost(m_dialog.takeIp(), m_dialog.takePort());

}

void LocalController::updateConnectionInfo()
{

    switch(m_protocol->socket()->state())
    {
    case QAbstractSocket::UnconnectedState:
        m_msgBox.setText("The socket is not connected ");
        break;
    case QAbstractSocket::HostLookupState:
        m_msgBox.setText("The socket is performing a host name lookup.");
        break;
    case QAbstractSocket::ConnectingState:
        m_msgBox.setText("The socket has started establishing a connection.");
        break;
    case QAbstractSocket::ConnectedState:
        initServerInfoItem();
        createGlobalChat();
        break;
    case QAbstractSocket::BoundState:
        m_msgBox.setText("The socket is bound to an address and port.");
        break;
    case QAbstractSocket::ListeningState:
        m_msgBox.setText("The socket is about to close (data may still be waiting to be written).");
        break;
    case QAbstractSocket::ClosingState:
        m_msgBox.setText("For internal use only.");
        break;
    }
    if(dynamic_cast<ServerInfoItem*>(m_mainWindow->listWidget()->currentItem()) == nullptr)
    {
        m_msgBox.exec();
    }
}

QString LocalController::parseMessage(const QString &message, const QString &src, const QString &color = "")
{
    QString parsedMessage = "";
    if(!color.isEmpty())
    {
        parsedMessage += "<font color=\"";
        parsedMessage += color;
        parsedMessage += "\">";
        parsedMessage += src;
        parsedMessage += ":";
        parsedMessage += NEW_LINE;
        parsedMessage += message;
        parsedMessage += "</font>";
    }
    else
    {
        parsedMessage += src;
        parsedMessage += ":";
        parsedMessage += NEW_LINE;
        parsedMessage += message;
    }
    parsedMessage += NEW_LINE;
    parsedMessage += NEW_LINE;
    return parsedMessage;
}
void LocalController::displayGlobalMessage(QString &message, QString &src)
{
    int idx = findItemIndex(src);
    if(idx == MY_ITEM)
        message = parseMessage(message, src, "#0000FF");
    else
        message = parseMessage(message, src);
    UserItem* item = reinterpret_cast<UserItem*>(m_mainWindow->listWidget()->item(GLOBAL_ITEM));
    item->appendMessage(message);
    if(item == m_mainWindow->listWidget()->currentItem())
    {
       m_mainWindow->chat()->setHtml(item->message());
       m_mainWindow->chat()->verticalScrollBar()->setValue(m_mainWindow->chat()->verticalScrollBar()->maximum());
    }
    else
    {
        emit newMessage(item->text());
    }
}

void LocalController::displayPrivateMessage(QString &message, QString &src, QString &target)
{
    int idxTarget = findItemIndex((src == m_serverInfoItem->myNickname()) ? target : src);
    int idxSrc = findItemIndex(src);
    UserItem *targetItem = reinterpret_cast<UserItem*>(m_mainWindow->listWidget()->item(idxTarget));
    if(idxSrc == MY_ITEM)
    {
        message = parseMessage(message, src, "#006600");
    }
    else
        message = parseMessage(message, src);
    targetItem->appendMessage(message);
    if(m_mainWindow->listWidget()->currentItem() == targetItem)
    {
        m_mainWindow->chat()->setHtml(targetItem->message());
        m_mainWindow->chat()->verticalScrollBar()->setValue(m_mainWindow->chat()->verticalScrollBar()->maximum());
    }
    else
    {
        emit newMessage(targetItem->text());
    }
}

void LocalController::signalNewMessage(const QString &tabName)
{
    int itemIdx = findItemIndex(tabName);
    QListWidget *lw = m_mainWindow->listWidget();
    if(itemIdx != lw->currentRow())
    {
        QListWidgetItem *it = lw->currentItem();
        lw->setCurrentRow(itemIdx);
        QTabBar *tb = m_mainWindow->tabBar();
        tb->setTabTextColor(tb->currentIndex(), QColor(0, 102, 0));
        lw->setCurrentItem(it);
    }
}
void LocalController::readChat()
{
    m_mainWindow->tabBar()->setTabTextColor(m_mainWindow->tabBar()->currentIndex(), QColor(0, 0, 0));
}

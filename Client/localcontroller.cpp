#include "localcontroller.h"
#include <QListWidget>
#include <QTextEdit>
#include <QTabBar>

LocalController::LocalController(QObject *parent, MainWindow *mw, Protocol *p) : QObject(parent), m_mainWindow(mw), m_protocol(p)
{
    m_msgBox.addButton(QMessageBox::StandardButton::Ok);
    initGuiController();
    connect(&m_dialog, &QDialog::accepted, this, &LocalController::provideConnectionData);
    connect(m_protocol->socket(), &QTcpSocket::connected, this, &LocalController::updateConnectionInfo);
    connect(m_protocol, &Protocol::userJoined, this, &LocalController::addUser);
    connect(m_protocol, &Protocol::userDisconnected, this, &LocalController::removeUser);
    connect(this, &LocalController::nicknameSetted, m_protocol, &Protocol::sendNicknameMessage);
}

void LocalController::initGuiController()
{
    connect(m_mainWindow->connectAction()->menu()->actions().at(0), &QAction::triggered, &m_dialog, &QDialog::exec);
    connect(m_mainWindow->connectAction()->menu()->actions().at(2), &QAction::triggered, this, &LocalController::disconnectFromHost);
    connect(m_mainWindow, &MainWindow::messageSent, this, &LocalController::sendMessage);
}

void LocalController::addUser(const QString &nickname)
{
    UserItem *item = new UserItem(nickname);
    m_mainWindow->listWidget()->addItem(item);
}
void LocalController::removeUser(const QString &nickname)
{
    UserItem *item = reinterpret_cast<UserItem*>(m_mainWindow->listWidget()->takeItem(findItemIndex(nickname)));
    m_mainWindow->listWidget()->removeItemWidget(item);
    if(item != nullptr)
        delete item;
    m_mainWindow->listWidget()->sortItems();
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
            setupAppToChatting();
        }
        else
        {
            m_serverInfoItem->appendMessage("Nickname is reapeted");
        }
    }
    else
    {
        reinterpret_cast<UserItem*>(lw->currentItem())->appendMessage(message);
        reinterpret_cast<UserItem*>(lw->currentItem())->appendMessage(NEW_LINE);
        m_mainWindow->chat()->setHtml(reinterpret_cast<UserItem*>(lw->currentItem())->message());
    }
}

void LocalController::initServerInfoItem()
{
    if(m_serverInfoItem == nullptr)
    {
        m_serverInfoItem = new ServerInfoItem(m_mainWindow->listWidget());
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

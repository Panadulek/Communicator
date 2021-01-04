#ifndef LOCALCONTROLLER_H
#define LOCALCONTROLLER_H
#include "mainwindow.h"
#include <QObject>
#include "connectiondialog.h"
#include "useritem.h"
#include "protocol.h"
#include "serverinfoitem.h"
#include <QMessageBox>

#define NEW_LINE "<br/>"

class LocalController : public QObject
{
    Q_OBJECT
    MainWindow *m_mainWindow;
    ConnectionDialog m_dialog;
    Protocol *m_protocol;
    ServerInfoItem *m_serverInfoItem = nullptr;
    QMessageBox m_msgBox;
    void setDialogLogic();
    int findItemIndex(const QString &);
    int findTabIndex(const QString &);
    void initGuiController();
    void initServerInfoItem();
    void createGlobalChat();
    void setupAppToChatting();
public:
    explicit LocalController(QObject *parent = nullptr, MainWindow *mw = nullptr, Protocol *p = nullptr);
signals:
void nicknameSetted(const QString &);

private slots:
	void swapMsg(QListWidgetItem*);
	void addTab(QListWidgetItem*);
	void changeTabData(int);
    void addUser(const QString &);
	void closeTab(int);
    void sendMessage(const QString &);
    void provideConnectionData();
    void updateConnectionInfo();
    void removeUser(const QString &);
    void disconnectFromHost();
};

#endif // LOCALCONTROLLER_H

#include "mainwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createMenuBar();
    createWidgets();
    setPosWidgets();
    m_tabBar->setTabsClosable(true);
    connect(m_senderWidget, &SenderWidget::messageSent, this, &MainWindow::messageSent);
}
void MainWindow::createMenuBar()
{
    m_menuBar = new QMenuBar(this);
    this->setMenuBar(m_menuBar);
    m_menuBar->addAction("Connect");
    QAction *a = connectAction();
    a->setMenu(new QMenu(this));
    a->menu()->addAction("new connection");
    a->menu()->addAction("disconnect");
}
void MainWindow::createWidgets()
{
    m_centralWidget = new QWidget(this);
    m_mainLayout = new QHBoxLayout(m_centralWidget);
    m_chat = new QTextEdit(this);
    m_chat->setReadOnly(true);
    m_senderWidget = new SenderWidget(this);
    m_vLayout1 = new QVBoxLayout;
    m_tabBar = new QTabBar(this);
    m_listWidget = new QListWidget(this);
}
void MainWindow::setPosWidgets()
{
    this->setCentralWidget(m_centralWidget);
    m_mainLayout->addLayout(m_vLayout1, 3);
    m_mainLayout->addWidget(m_listWidget, 1);
    m_vLayout1->addWidget(m_tabBar, 1);
    m_vLayout1->addWidget(m_chat, 2);
    m_vLayout1->addWidget(m_senderWidget);
}

void MainWindow::keyPressEvent(QKeyEvent *ev)
{
    if(ev->key()==Qt::Key_Return)
    {
        m_senderWidget->button()->click();
    }
    QMainWindow::keyPressEvent(ev);
}
MainWindow::~MainWindow()
{
    delete m_vLayout1;
}


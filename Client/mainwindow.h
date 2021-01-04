#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QListWidget>
#include <QMainWindow>
#include <QTabBar>
#include <QTabWidget>
#include "senderwidget.h"
#include <QTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListWidget>
#include <QMenuBar>
#include <QKeyEvent>


#include "useritem.h"
class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    SenderWidget *m_senderWidget;
    QWidget *m_centralWidget;
    QTextEdit *m_chat;
    QHBoxLayout *m_mainLayout;
    QVBoxLayout *m_vLayout1;
    QTabBar *m_tabBar;
    QListWidget *m_listWidget;
    QMenuBar *m_menuBar;
    void createWidgets();
    void setPosWidgets();
    void createMenuBar();
    void keyPressEvent(QKeyEvent *ev) override;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QAction *connectAction(){return m_menuBar->actions().at(0);}
    QListWidget* listWidget(){return m_listWidget;}
    QTextEdit* chat(){return m_chat;}
    QTabBar* tabBar(){return m_tabBar;}
    SenderWidget *senderWidget(){return m_senderWidget;}
signals:
    void messageSent(const QString &);
};
#endif // MAINWINDOW_H

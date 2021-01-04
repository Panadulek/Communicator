#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H
#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
class ConnectionDialog : public QDialog
{
    Q_OBJECT
private:
    QLineEdit *m_ipEdit;
    QLineEdit *m_portEdit;
    QLabel *m_ipLabel;
    QLabel *m_portLabel;
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_ipLayout;
    QHBoxLayout *m_portLayout;
    QHBoxLayout *m_buttonLayout;
    QPushButton *m_buttonAccept;
    QPushButton *m_buttonCancel;
    void createWidgets();
    void setPosWidgets();
public:
    explicit ConnectionDialog(QWidget *parent = nullptr);
    QString takeIp()
    {
        QString ret = m_ipEdit->text();
        m_ipEdit->clear();
        return ret;
    }
    QString takePort()
    {
        QString ret = m_portEdit->text();
        m_portEdit->clear();
        return ret;
    }
signals:

};

#endif // CONNECTIONWIDGET_H

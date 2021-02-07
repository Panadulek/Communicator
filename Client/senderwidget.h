#ifndef SENDERWIDGET_H
#define SENDERWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLineEdit>
class SenderWidget : public QWidget
{
    Q_OBJECT
    QHBoxLayout *m_layout;
    QLineEdit *m_lineEdit;
    QPushButton *m_button;
public:
    explicit SenderWidget(QWidget *parent = nullptr);
    QLineEdit *lineEdit(){return m_lineEdit;}
    QPushButton *button(){return m_button;}
signals:
    void messageSent(const QString &);
};

#endif // SENDERWIDGET_H

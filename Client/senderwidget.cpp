#include "senderwidget.h"

SenderWidget::SenderWidget(QWidget *parent) : QWidget(parent)
{
    m_layout = new QHBoxLayout(this);
    m_button = new QPushButton("send", this);
    m_lineEdit = new QLineEdit(this);
    m_layout->addWidget(m_lineEdit, 2);
    m_layout->addWidget(m_button, 1);
    connect(m_button, &QPushButton::clicked, this, [this]()
	{
        emit messageSent(m_lineEdit->text());
	});
    m_button->setDefault(true);
}

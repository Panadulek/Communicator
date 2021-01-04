#include "connectiondialog.h"

ConnectionDialog::ConnectionDialog(QWidget *parent) : QDialog(parent)
{
    createWidgets();
    setPosWidgets();
    connect(m_buttonAccept, &QPushButton::clicked, this, &QDialog::accept);
    connect(m_buttonCancel, &QPushButton::clicked, this, &QDialog::reject);
}

void ConnectionDialog::createWidgets()
{
    m_ipEdit = new QLineEdit(this);
    m_portEdit = new QLineEdit(this);
    m_ipLabel = new QLabel("Ip", this);
    m_portLabel = new QLabel("Port", this);
    m_mainLayout = new QVBoxLayout(this);
    m_ipLayout = new QHBoxLayout();
    m_portLayout = new QHBoxLayout();
    m_buttonLayout = new QHBoxLayout();
    m_buttonAccept = new QPushButton("accept", this);
    m_buttonCancel = new QPushButton("cancel", this);
}

void ConnectionDialog::setPosWidgets()
{
    m_ipLayout->addWidget(m_ipLabel);
    m_ipLayout->addWidget(m_ipEdit);
    m_portLayout->addWidget(m_portLabel);
    m_portLayout->addWidget(m_portEdit);
    m_buttonLayout->addWidget(m_buttonAccept);
    m_buttonLayout->addWidget(m_buttonCancel);
    m_mainLayout->addLayout(m_ipLayout);
    m_mainLayout->addLayout(m_portLayout);
    m_mainLayout->addLayout(m_buttonLayout);
}

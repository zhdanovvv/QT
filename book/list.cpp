#include "list.h"
#include <QTextStream>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <QLabel>
#include <QMessageBox>
#include <QApplication>
#include <QSpacerItem>
#include <memory>


User::User(QWidget* parent)
        : QWidget(parent)
{
    QVBoxLayout* vLayout(new QVBoxLayout);
    QHBoxLayout* hLayout(new QHBoxLayout);
    QHBoxLayout* twohLayout(new QHBoxLayout);
    QPushButton* say_hello = new QPushButton("SayHello");
    QPushButton* close_btn = new QPushButton("Close");
    QLabel* nameLbl = new QLabel("Name:");
    QLineEdit* name = new QLineEdit;
    hLayout->addWidget(nameLbl);
    hLayout->addWidget(name);
    hLayout->setAlignment(Qt::AlignTop);
    twohLayout->addWidget(say_hello, 1, Qt::AlignRight | Qt::AlignBottom);
    twohLayout->insertSpacing(2, 0);
    twohLayout->addWidget(close_btn);
    connect(close_btn, &QPushButton::clicked, qApp, &QApplication::quit);
    connect(say_hello, &QPushButton::clicked, this, &User::message);
    setLayout(vLayout);
    vLayout->addLayout(hLayout);
    vLayout->insertSpacing(2, 100);
    vLayout->addLayout(twohLayout);
}


void User::message()
{
    QString textEdit = "Hello ";
    QMessageBox* box_window = new QMessageBox;
    box_window->setWindowTitle("Hello");
    box_window->setText(textEdit);
    box_window->show();
}






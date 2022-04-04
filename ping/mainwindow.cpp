#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QClipboard>
#include <QDebug>
#include <QProgressBar>
#include <QHostInfo>
#include <QFile>
#include <QDateEdit>
#include <QTextCursor>
#include <QTextList>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->progressBar->reset();
    ui->dateEdit->setDate(QDate::currentDate());
    connect(ui->ping_button, SIGNAL(clicked()), this, SLOT(Ping()));
    connect(ui->write_button, SIGNAL(clicked()), this, SLOT(WriteFile()));
    connect(ui->read_button, SIGNAL(clicked()), this, SLOT(ReadFile()));
    connect(ui->add_ip_button, SIGNAL(clicked()), this, SLOT(Add_ip_intext_edit()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Add_ip_intext_edit()
{
   ui->lineEdit_add_ip->setInputMask("000.000.000.000");
   QHostAddress ip;
   if(ip.setAddress(ui->lineEdit_add_ip->text()))
   {
        qDebug() << "Valid IP Address";
        ui->lineEdit_add_ip->setText(ip.toString());
        QString ip_adress = ui->lineEdit_add_ip->text();
        QTextCursor textEdit_cursor = ui->textEdit_ping_ip->textCursor();
        textEdit_cursor.movePosition(QTextCursor::StartOfBlock);
        ui->lineEdit_add_ip->clear();
        ui->textEdit_ping_ip->insertPlainText(ip_adress);
        list_ip.append(ip_adress);
        textEdit_cursor.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor);
        textEdit_cursor.insertBlock();
   }
   else
   {
        qDebug() << "Invalid IP address";
   }
}

void MainWindow::Ping()
{    
    ui->progressBar->reset();
    QList<QString>::iterator i;
    for(i = list_ip.begin(); i != list_ip.end(); i++)
    {
        QHostInfo infoName = QHostInfo::fromName(*i);
        if(infoName.error() == QHostInfo::HostNotFound)
        {
            qDebug() << "Host not found";
            qDebug() << *i;
            ui->label_result->setText("Result ping - IP: Error - Host not found");
            result_ping.append("Result ping - IP: Error - Host not found");
            ui->progressBar->setValue(ui->progressBar->maximum());
        }
        else if(infoName.error() == QHostInfo::NoError)
        {
            qDebug() << "IP:" << infoName.hostName() << infoName.addresses();
            ui->label_result->setText("Result ping - Found address: IP - " + infoName.hostName());
            result_ping.append("Result ping - Found address: IP - " + infoName.hostName());
            ui->progressBar->setValue(ui->progressBar->maximum());
        }
    }
}

void MainWindow::WriteFile()
{
    QFile file("result.txt");
    if(ui->file_radiobutton->isChecked() == true)
    {        
        if(file.exists())
        {
            qDebug() << "file exists";
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                return;
            }
            QTextStream out(&file);
            QList<QString>::iterator i;
            for(i = result_ping.begin(); i != result_ping.end(); i++)
            {
                out << *i << "\n";
            }
            file.close();
        }
        else
        {
            qDebug() << "file not exists";
            QFile file("result.txt");
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                return;
            }
            QTextStream out(&file);
            QList<QString>::iterator i;
            for(i = result_ping.begin(); i != result_ping.end(); i++)
            {
                out << *i << "\n";
            }
            file.close();
        }
    }
    else
    {
        qDebug() << "Check file button";
    }
}

void MainWindow::ReadFile()
{
    if(ui->file_radiobutton->isChecked() == true)
    {
        QFile file("result.txt");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            return;
        }
        QTextStream in(&file);
        QString line;
        while (!in.atEnd())
        {
            line = in.readLine();
            qDebug() << line;
        }
        file.close();
    }
    else
    {
        qDebug() << "Check file button";
    }
}

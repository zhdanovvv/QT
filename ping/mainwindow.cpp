#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QClipboard>
#include <QDebug>
#include <QProgressBar>
#include <QHostInfo>
#include <QFile>
#include <QDate>
#include <QTextCursor>
#include <QTextList>
#include <QProcess>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->progressBar->reset();
    QDate dt = QDate::currentDate();
    ui->label_data_time->setText(dt.toString(Qt::ISODate));
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("ping_db");
    if(db.open())
    {
        qDebug() << "Ping_db is open";
        QSqlQuery query(db);
        query.prepare("CREATE TABLE IF NOT EXISTS info_ping (adress_id INTEGER PRIMARY KEY AUTOINCREMENT, adress TEXT NOT NULL, request_info TEXT NOT NULL)");
        if(query.exec())
        {
             qDebug() << query.lastQuery();
        }
    }
    db.close();
    connect(ui->ping_button, SIGNAL(clicked()), this, SLOT(Ping()));
    connect(ui->write_button, SIGNAL(clicked()), this, SLOT(WriteFile()));
    connect(ui->read_button, SIGNAL(clicked()), this, SLOT(ReadFile()));
    connect(ui->write_button, SIGNAL(clicked()), this, SLOT(WriteDataBase()));
    connect(ui->read_button, SIGNAL(clicked()), this, SLOT(ReadDataBase()));
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
        const QString ip_adress = ui->lineEdit_add_ip->text();
        QTextCursor textEdit_cursor = ui->textEdit_ping_ip->textCursor();
        textEdit_cursor.movePosition(QTextCursor::StartOfBlock);
        ui->lineEdit_add_ip->clear();
        if(!list_ip.contains(ip_adress))
        {
            list_ip << ip_adress;
            ui->textEdit_ping_ip->insertPlainText(ip_adress);
            qDebug() << list_ip;
            textEdit_cursor.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor);
            textEdit_cursor.insertBlock();
        }
        else
        {
            if(!ui->textEdit_ping_ip->find(ip_adress))
            {
                ui->textEdit_ping_ip->insertPlainText(ip_adress);
                textEdit_cursor.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor);
                textEdit_cursor.insertBlock();
            }
        }
   }
   else
   {
        qDebug() << "Invalid IP address";
   }
}

void MainWindow::Ping()
{
    ui->ping_result->clear();
    QProcess* ping = new QProcess();
    int value_progress_bar;
    int add_value_progress_bar;
    bool yes = true;
    const QString command_ping = "ping";
    QString c_arg = "-c";
    QString w_arg = "-w";
    QString count_packet = "1";
    QString ping_timeout = "10";    
    QString result;
    QString clipboard_text;
    QClipboard* clipboard = QGuiApplication::clipboard();
    QStringList new_list_ip;
    ui->progressBar->reset();    
    clipboard->clear();
    ui->textEdit_ping_ip->copyAvailable(yes);
    ui->textEdit_ping_ip->copy();
    clipboard_text = clipboard->text();
    new_list_ip << clipboard_text.split(QLatin1Char('\n'));
    for(int i = 0; i < new_list_ip.size(); i++)
    {
        qDebug() << new_list_ip;
        value_progress_bar = 100 / list_ip.size();
        ping->setProgram(command_ping);
        ping->setProcessChannelMode(QProcess::MergedChannels);
        ping->start(command_ping, QStringList(new_list_ip.at(i)) << c_arg << count_packet << w_arg << ping_timeout);
        if (!ping->waitForReadyRead())
        {
            qDebug() << "Ping failed:" << ping->errorString();
        }
        else
        {
            result = QString(ping->readAllStandardOutput());
            if(result.isEmpty())
            {
                qDebug() << "there is not data";
            }
            else
            {
                qDebug() << "there is data" << result;
                if(!result_ping.contains(result))
                {
                    result_ping.append(result);
                }
                ui->ping_result->insertPlainText(result + "\n");
                ui->progressBar->setValue(value_progress_bar);                
            }
        }
        ping->close();
    }
    add_value_progress_bar = 100 - ui->progressBar->value();
    ui->progressBar->setValue(ui->progressBar->value() + add_value_progress_bar);
}

void MainWindow::WriteFile()
{
    QFile file("result.txt");
    if(ui->file_radiobutton->isChecked() == true)
    {
        QMessageBox* message = new QMessageBox;
        message->setIcon(QMessageBox::Information);
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
                if(i == result_ping.end()--)
                {
                    message->setText("Data were written in file ");
                }
            }
            file.close();
            message->show();
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
                if(i == result_ping.end()--)
                {
                    message->setText("Data were written in file ");
                }
            }
            file.close();
            message->show();
        }
    }    
}

void MainWindow::ReadFile()
{
    if(ui->file_radiobutton->isChecked() == true)
    {
        QMessageBox* message = new QMessageBox;
        message->setIcon(QMessageBox::Information);
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
            message->setText(message->text() + line + "\n");
            message->show();
            //qDebug() << line;
        }
        file.close();
    } 
}

void MainWindow::WriteDataBase()
{
    if(ui->base_radiobutton->isChecked() == true)
    {
        QMessageBox* message = new QMessageBox;
        message->setIcon(QMessageBox::Information);
        if(db.open())
        {
            QSqlQuery insert(db);
            for(int i = 0; i < list_ip.size(); i++)
            {
                insert.prepare("INSERT INTO info_ping (adress ,request_info) VALUES (:adress, :request_info)");
                insert.bindValue(":adress", list_ip[i]);
                insert.bindValue(":request_info", "");
                if(insert.exec())
                {
                     qDebug() << insert.lastQuery();
                     if(i == list_ip.size() - 1)
                     {
                        message->setText("Data were written in database ");
                     }
                }
                else
                {
                    qDebug() << insert.lastError();
                }
            }
            for(int i = 0; i < result_ping.size(); i++)
            {
                insert.prepare("INSERT INTO info_ping (adress, request_info) VALUES (:adress, :request_info)");
                insert.bindValue(":request_info", result_ping[i]);
                insert.bindValue(":adress", "");
                if(insert.exec())
                {
                     qDebug() << insert.lastQuery();
                     if(i == result_ping.size() - 1)
                     {
                        message->setText("Data were written in database");
                     }
                }
                else
                {
                    qDebug() << insert.lastError();
                }
            }
            db.close();
        }
        else if(!db.open())
        {
            QString ping_db_error = db.lastError().text();
            qDebug() << ping_db_error;
        }
        message->show();
    }
}

void MainWindow::ReadDataBase()
{    
    if(ui->base_radiobutton->isChecked() == true)
    {
        QMessageBox* message = new QMessageBox;
        message->setIcon(QMessageBox::Information);
        if(db.open())
        {
            QStringList tables_ping_db = db.tables();
            qDebug() << "Ping_db is open" << "\n" << tables_ping_db;
            QSqlQuery select(db);
            select.prepare("SELECT request_info FROM info_ping");
            if(select.exec())
            {
                 qDebug() << select.lastQuery();
            }
            while (select.next())
            {                
                //qDebug() << select.value("request_info").toString() << "\n";
                message->setText(message->text() + select.value("request_info").toString() + "\n");
                message->show();
            }
            db.close();
        }
        else if(!db.open())
        {
            QString ping_db_error = db.lastError().text();
            //qDebug() << ping_db_error;
            message->setText(ping_db_error);
            message->show();
        }
    }   
}


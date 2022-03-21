#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QClipboard>
#include <QDebug>
#include <QProgressBar>
#include <QHostInfo>
#include <QFile>
#include <QDateEdit>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->progressBar->reset();
    ui->dateEdit->setDate(QDate::currentDate());
    connect(ui->ping_button, SIGNAL(released()), this, SLOT(Ping()));
    connect(ui->write_button, SIGNAL(clicked()), this, SLOT(WriteFile()));
    connect(ui->read_button, SIGNAL(clicked()), this, SLOT(ReadFile()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Ping()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    const QString clipboardText = clipboard->text();
    QHostInfo infoName =  QHostInfo::fromName(clipboardText);
    if(infoName.error() == QHostInfo::HostNotFound)
    {
        qDebug() << "Host not found";
        ui->label_result->setText("Result ping - IP: Error - Host not found");
        ui->progressBar->setValue(ui->progressBar->maximum());
    }
    else if(infoName.error() == QHostInfo::NoError)
    {
        qDebug() << "IP:" << infoName.hostName() << infoName.addresses();
        ui->label_result->setText("Result ping - Found address: IP - " + infoName.hostName());
        ui->progressBar->setValue(ui->progressBar->maximum());
    }
}

void MainWindow::WriteFile()
{
    if(ui->file_radiobutton->isChecked() == true)
    {
        QFile file("result.txt");
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;

        QTextStream out(&file);
        out << ui->label_result->text() << "\n";
        file.close();
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
            return;

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

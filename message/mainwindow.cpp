#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_2_clicked()
{
    QString text = "Guten Tag " + ui->nameLineEdit->displayText() + "!!!";
    QMessageBox* message = new QMessageBox;
    message->resize(1000, 1000);
    message->setWindowTitle("Saying window");
    message->setText(text);
    message->show();
}



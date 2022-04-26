#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QHostInfo>
#include <QList>
#include <QStringList>
#include <QSqlDatabase>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void Ping();
    void WriteFile();
    void ReadFile();
    void Add_ip_intext_edit();
    void WriteDataBase();
    void ReadDataBase();   

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QStringList list_ip;
    QStringList result_ping;
};
#endif // MAINWINDOW_H

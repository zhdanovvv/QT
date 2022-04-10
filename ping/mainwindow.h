#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QHostInfo>
#include <QList>
#include <QStringList>

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

private:
    Ui::MainWindow *ui;
    QStringList list_ip;
    QList<QString> result_ping;
};
#endif // MAINWINDOW_H

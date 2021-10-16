#ifndef LIST_H
#define LIST_H

#include <QWidget>


class User:public QWidget
{

public:
    User(QWidget* parent = nullptr);


private slots:
    void message();

private:

};

#endif // LIST_H

#ifndef MYAPPLICATION_H
#define MYAPPLICATION_H

#include <QApplication>

class MyApplication : public QApplication
{
public:
    bool notify(QObject *receiver, QEvent *event);
    MyApplication(int &argc, char **argv);
};

#endif // MYAPPLICATION_H

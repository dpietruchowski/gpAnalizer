#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

public:
    Ui::MainWindow *ui_;
    int counter_;

signals:

public slots:
    void newApplication();
    void deleteApplication();
    void copyApplication();
};

#endif // MAINWINDOW_H

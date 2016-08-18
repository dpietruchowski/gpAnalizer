#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <application/application.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsPixmapItem *item;
    application app;

private slots:
    void applicationStart();
    void applicationStop();

    void setOperationName(std::string operationName);
    void setBestProgram(bestIndividual best);

private:
    QImage cvMatToQImage(const cv::Mat &inMat);
    QPixmap cvMatToQPixmap(const cv::Mat &inMat);
};

#endif // MAINWINDOW_H

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
    Ui::MainWindow *ui_;
    QGraphicsScene *scene_;
    QGraphicsPixmapItem *item_;
    Application app_;

    bool isInputImage_;
    bool isReferenceImage_;
    bool isOptionSetup_;

private slots:
    void applicationStart();
    void applicationStop();

    void setOperationName(std::string operationName);
    void setBestProgram(BestIndividual best);

    void on_inputImageButton_clicked();
    void on_referenceImageButton_clicked();
    void on_saveFolderButton_clicked();
    void on_optionsButton_clicked();
    void on_resetButton_clicked();

private:
    QImage cvMatToQImage(const cv::Mat &inMat);
    QPixmap cvMatToQPixmap(const cv::Mat &inMat);

    void buttonsEnabledStart();
    void buttonsEnabledStop();
    void buttonsEnabledReset();
};

#endif // MAINWINDOW_H

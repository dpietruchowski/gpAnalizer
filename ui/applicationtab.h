#ifndef ApplicationTab_H
#define ApplicationTab_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <application/application.h>
#include "ui/settings.h"

namespace Ui {
class ApplicationTab;
}

class ApplicationTab : public QWidget
{
    Q_OBJECT

public:
    explicit ApplicationTab(QWidget *parent = 0);
    ~ApplicationTab();

    void setApplication(const Application& rhs);
    const Application &getApplication();


private:
    Ui::ApplicationTab *ui_;
    QGraphicsScene *scene_;
    QGraphicsPixmapItem *item_;
    Application app_;
    Settings *setting_;
    bool isInputImage_;
    bool isReferenceImage_;
    bool isOptionSetup_;
    void getKatalog(std::string& katalog);
    void buttonsEnabledStart();

private slots:
    void applicationStart();
    void applicationStop();

    void setOperationName(std::string operationName);
    void setBestProgram(BestIndividual best);
    void setSettings(const Setting& settings);

    void on_inputImageButton_clicked();
    void on_referenceImageButton_clicked();
    void on_saveFolderButton_clicked();
    void on_optionsButton_clicked();
    void on_resetButton_clicked();

private:
    QImage cvMatToQImage(const cv::Mat &inMat);
    QPixmap cvMatToQPixmap(const cv::Mat &inMat);

    void buttonsEnabledStop();
    void buttonsEnabledReset();
};

#endif // ApplicationTab_H

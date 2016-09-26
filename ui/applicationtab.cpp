#include "applicationtab.h"
#include "ui_applicationtab.h"

#include <string>
#include <iostream>
#include <QFileDialog>
#include <QMessageBox>
#include <opencv2/highgui/highgui.hpp>

using namespace std;

ApplicationTab::ApplicationTab(QWidget *parent) :
    QWidget(parent), ui_(new Ui::ApplicationTab),
    isInputImage_(false), isReferenceImage_(false), isOptionSetup_(false)
{
    ui_->setupUi(this);
    setting_ = new Settings(this);
    setting_->hide();
    scene_ = new QGraphicsScene();
    ui_->bestImageView->setScene(scene_);
    item_ = new QGraphicsPixmapItem();
    scene_->addItem(item_);

    QObject::connect(ui_->startButton, SIGNAL(clicked()), this, SLOT(applicationStart()));
    QObject::connect(ui_->stopButton, SIGNAL(clicked()), this, SLOT(applicationStop()));
    QObject::connect(setting_, SIGNAL(getSettings(const Setting&)), this, SLOT(setSettings(const Setting&)));

    isInputImage_ = true;
    isReferenceImage_ = true;
    buttonsEnabledStart();

    for(int i = 0; i < nApps; i++)
    {
        apps_.push_back(new Application());
    }

    string inputName = "input.png";
    cv::Mat inputImage = cv::imread(inputName, 0);
    for(int i = 0; i < nApps; i++)
    {
        apps_[i]->setInputImage(inputImage);
    }
    QGraphicsScene *scene = new QGraphicsScene();
    ui_->inputImageView->setScene(scene);
    QGraphicsPixmapItem *item = new QGraphicsPixmapItem();
    scene->addItem(item);
    item->setPixmap(cvMatToQPixmap(inputImage));
    ui_->inputImageView->fitInView(item);
    ui_->inputImageView->show();

    string referenceName = "reference.png";
    cv::Mat referenceImage = cv::imread(referenceName, 0);

    for(int i = 0; i < nApps; i++)
    {
        apps_[i]->setReferenceImage(referenceImage);
    }

    QGraphicsScene *scene2 = new QGraphicsScene();
    ui_->referenceImageView->setScene(scene2);
    QGraphicsPixmapItem *item2 = new QGraphicsPixmapItem();
    scene2->addItem(item2);
    item2->setPixmap(cvMatToQPixmap(referenceImage));
    ui_->referenceImageView->fitInView(item2);
    ui_->referenceImageView->show();
}

ApplicationTab::~ApplicationTab()
{
    delete ui_;
    delete scene_;
    delete setting_;
}

void ApplicationTab::applicationStart()
{
    for(int i = 0; i < nApps; i++)
    {
        apps_[i]->start();
    }

    //QObject::disconnect(ui_->startButton, SIGNAL(clicked()), this, SLOT(applicationStart()));

    QObject::connect(apps_[0], SIGNAL(getGeneration(int)), ui_->generationNumber, SLOT(display(int)));
    QObject::connect(apps_[0], SIGNAL(getAssessed(int)), ui_->individualNumber, SLOT(display(int)));
    qRegisterMetaType<std::string>("std::string");
    QObject::connect(apps_[0], SIGNAL(getOperation(std::string)), this, SLOT(setOperationName(std::string)));
    qRegisterMetaType<BestIndividual>("bestIndividual");
    QObject::connect(apps_[0], SIGNAL(newBestProgram(BestIndividual)), this, SLOT(setBestProgram(BestIndividual)));

    buttonsEnabledStop();
}

void ApplicationTab::applicationStop()
{
//   app_.stop();

    for(int i = 0; i < nApps; i++)
    {
        apps_[i]->terminate();
    }

    //QObject::disconnect(ui_->stopButton, SIGNAL(clicked()), this, SLOT(applicationStop()));
    buttonsEnabledReset();
}

void ApplicationTab::setOperationName(std::string operationName)
{
    ui_->operationNameLabel->setText(QString::fromStdString(operationName));

}

void ApplicationTab::setBestProgram(BestIndividual best)
{
    ui_->resultNumberBest->display(best.programResult);
    ui_->generationNumberBest->display(best.generationNumber);
    ui_->individualNumberBest->display(best.individualNumber);

    cv::Mat image;
    threshold(best.image, image, 125, 255, 0);

    item_->setPixmap(cvMatToQPixmap(image));
    //scene->addItem(item);
    ui_->bestImageView->fitInView(item_);
    ui_->bestImageView->show();
}

void ApplicationTab::setSettings(const Setting &settings)
{
    for(int i = 0; i < nApps; i++)
    {
        apps_[i]->setGeneticOperationProbabilities(settings.geneticOperationProbabilities);
        apps_[i]->setGeneticParameters(settings.geneticParam);
        apps_[i]->setNodeProbabilities(settings.geneticNodeProbabilities);
        apps_[i]->setStopCriterium(settings.stopCriteriumParameters);
    }
}

QImage  ApplicationTab::cvMatToQImage(const cv::Mat &inMat)
   {
      switch ( inMat.type() )
      {
         // 8-bit, 4 channel
         case CV_8UC4:
         {
            QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB32 );

            return image;
         }

         // 8-bit, 3 channel
         case CV_8UC3:
         {
            QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB888 );

            return image.rgbSwapped();
         }

         // 8-bit, 1 channel
         case CV_8UC1:
         {
            static QVector<QRgb>  sColorTable;

            // only create our color table once
            if ( sColorTable.isEmpty() )
            {
               for ( int i = 0; i < 256; ++i )
                  sColorTable.push_back( qRgb( i, i, i ) );
            }

            QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_Indexed8 );

            image.setColorTable( sColorTable );

            return image;
         }

         default:
            qWarning() << "ASM::cvMatToQImage() - cv::Mat image type not handled in switch:" << inMat.type();
            break;
      }

      return QImage();
   }

QPixmap ApplicationTab::cvMatToQPixmap(const cv::Mat &inMat)
{
    return QPixmap::fromImage( cvMatToQImage( inMat ) );
}

void ApplicationTab::on_inputImageButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    QDir::currentPath(),
                                                    tr("Images (*.png *.jpg)"));
    if(fileName.isEmpty() == true)
        return;

    string inputName = fileName.toStdString();
    cv::Mat inputImage = cv::imread(inputName, 0);

    for(int i = 0; i < nApps; i++)
    {
        apps_[i]->setInputImage(inputImage);
    }

    QGraphicsScene *scene = new QGraphicsScene();
    ui_->inputImageView->setScene(scene);
    QGraphicsPixmapItem *item = new QGraphicsPixmapItem();
    scene->addItem(item);
    item->setPixmap(cvMatToQPixmap(inputImage));
    ui_->inputImageView->fitInView(item);
    ui_->inputImageView->show();

    isInputImage_ = true;
    buttonsEnabledStart();
}

void ApplicationTab::on_referenceImageButton_clicked()
{

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    QDir::currentPath(),
                                                    tr("Images (*.png *.jpg)"));

    if(fileName.isEmpty() == true)
        return;

    string referenceName = fileName.toStdString();
    cv::Mat referenceImage = cv::imread(referenceName, 0);
    for(int i = 0; i < nApps; i++)
    {
        apps_[i]->setReferenceImage(referenceImage);
    }

    QGraphicsScene *scene = new QGraphicsScene();
    ui_->referenceImageView->setScene(scene);
    QGraphicsPixmapItem *item = new QGraphicsPixmapItem();
    scene->addItem(item);
    item->setPixmap(cvMatToQPixmap(referenceImage));
    ui_->referenceImageView->fitInView(item);
    ui_->referenceImageView->show();

    isReferenceImage_ = true;
    buttonsEnabledStart();
}

void ApplicationTab::on_saveFolderButton_clicked()
{
    QDir saveDir = QFileDialog::getExistingDirectory(this, tr("Open File"),
                                                QDir::currentPath());
    if(saveDir == QDir::currentPath())
        return;
    QString saveString = saveDir.path();
    if(saveString == ".")
        return;

    while(saveDir.entryInfoList(QDir::NoDotAndDotDot|
                                QDir::AllEntries).count() != 0)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Ostrzezenie",
                                    "Zawartosc folderu bedzie usunieta\n"
                                    "Czy chcesz kontynuowac?",
                                    QMessageBox::Yes|QMessageBox::No);

        if(reply == QMessageBox::No)
        {
            saveDir = QFileDialog::getExistingDirectory(
                            this, tr("Open File"), QDir::currentPath());
            if(saveDir == QDir::currentPath())
                return;
            saveString = saveDir.path();
            if(saveString == "")
                return;
        }
        else break;

    }

    string savePath = saveString.toStdString();
    for(int i = 0; i < nApps; i++)
    {
        string ssp = savePath + "/" + to_string(i+1);
        apps_[i]->setKatalog(ssp);
    }
    isOptionSetup_ = true;
    buttonsEnabledStart();
}

void ApplicationTab::on_optionsButton_clicked()
{
    setting_->setLineEdits();
    setting_->setModal(true);
    setting_->exec();
}

void ApplicationTab::on_resetButton_clicked()
{
    for(int i = 0; i < nApps; i++)
    {
        *apps_[i] = *apps_[i];
    }

    buttonsEnabledStart();
}

void ApplicationTab::buttonsEnabledStart()
{
    ui_->stopButton->setEnabled(false);
    ui_->startButton->setEnabled(isInputImage_
                                 & isReferenceImage_
                                 & isOptionSetup_);
    ui_->inputImageButton->setEnabled(true);
    ui_->referenceImageButton->setEnabled(true);
    ui_->saveFolderButton->setEnabled(true);
    ui_->optionsButton->setEnabled(true);
    ui_->resetButton->setEnabled(false);
}

void ApplicationTab::buttonsEnabledStop()
{
    ui_->stopButton->setEnabled(true);
    ui_->startButton->setEnabled(false);
    ui_->inputImageButton->setEnabled(false);
    ui_->referenceImageButton->setEnabled(false);
    ui_->saveFolderButton->setEnabled(false);
    ui_->optionsButton->setEnabled(false);
    ui_->resetButton->setEnabled(false);
}

void ApplicationTab::buttonsEnabledReset()
{
    ui_->stopButton->setEnabled(false);
    ui_->startButton->setEnabled(false);
    ui_->inputImageButton->setEnabled(false);
    ui_->referenceImageButton->setEnabled(false);
    ui_->saveFolderButton->setEnabled(false);
    ui_->optionsButton->setEnabled(false);
    ui_->resetButton->setEnabled(false);
}

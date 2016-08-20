#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene();
    ui->bestImageView->setScene(scene);
    item = new QGraphicsPixmapItem();
    scene->addItem(item);

    QObject::connect(ui->startButton, SIGNAL(clicked()), this, SLOT(applicationStart()));
    QObject::connect(ui->stopButton, SIGNAL(clicked()), this, SLOT(applicationStop()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::applicationStart()
{
    app.start();
    QObject::disconnect(ui->startButton, SIGNAL(clicked()), this, SLOT(applicationStart()));

    QObject::connect(&app, SIGNAL(getGeneration(int)), ui->generationNumber, SLOT(display(int)));
    QObject::connect(&app, SIGNAL(getIndividual(int)), ui->individualNumber, SLOT(display(int)));
    qRegisterMetaType<std::string>("std::string");
    QObject::connect(&app, SIGNAL(getOperation(std::string)), this, SLOT(setOperationName(std::string)));
    qRegisterMetaType<bestIndividual>("bestIndividual");
    QObject::connect(&app, SIGNAL(newBestProgram(bestIndividual)), this, SLOT(setBestProgram(bestIndividual)));
}

void MainWindow::applicationStop()
{
    app.stop();
    QObject::disconnect(ui->stopButton, SIGNAL(clicked()), this, SLOT(applicationStop()));
}

void MainWindow::setOperationName(std::string operationName)
{
    ui->operationNameLabel->setText(QString::fromStdString(operationName));

}

void MainWindow::setBestProgram(bestIndividual best)
{
    ui->resultNumberBest->display(best.programResult);
    ui->generationNumberBest->display(best.generationNumber);
    ui->individualNumberBest->display(best.individualNumber);

    item->setPixmap(cvMatToQPixmap(best.image));
    //scene->addItem(item);
    ui->bestImageView->fitInView(item);
    ui->bestImageView->show();
}

QImage  MainWindow::cvMatToQImage(const cv::Mat &inMat)
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

QPixmap MainWindow::cvMatToQPixmap(const cv::Mat &inMat)
{
  return QPixmap::fromImage( cvMatToQImage( inMat ) );
}

void MainWindow::on_inputImageButton_clicked()
{

}

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "applicationtab.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent), ui_(new Ui::MainWindow)
{
    ui_->setupUi(this);
    counter_ = 0;

    QObject::connect(ui_->newApp, SIGNAL(triggered()),
                     this, SLOT(newApplication()));
    QObject::connect(ui_->deleteApp, SIGNAL(triggered()),
                     this, SLOT(deleteApplication()));
    QObject::connect(ui_->copyApp, SIGNAL(triggered()),
                     this, SLOT(copyApplication()));
}

void MainWindow::newApplication()
{
    ui_->tabWidget->addTab(new ApplicationTab(this), "App");
}

void MainWindow::deleteApplication()
{
    int current = ui_->tabWidget->currentIndex();
    ui_->tabWidget->removeTab(current);
}

void MainWindow::copyApplication()
{
    QWidget* current = ui_->tabWidget->currentWidget();
    ApplicationTab* newWidget = new ApplicationTab(this);

    newWidget->setApplication(dynamic_cast<ApplicationTab*> (current)->getApplication());

//    string* katalog;
//    newWidget->getKatalog(*katalog);

//    counter_++;
//    *katalog += to_string(counter_);

    ui_->tabWidget->addTab(newWidget, "App");
}


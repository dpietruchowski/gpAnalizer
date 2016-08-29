#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "applicationtab.h"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent), ui_(new Ui::MainWindow)
{
    ui_->setupUi(this);

    QObject::connect(ui_->newApp, SIGNAL(triggered()),
                     this, SLOT(newApplication()));
    QObject::connect(ui_->deleteApp, SIGNAL(triggered()),
                     this, SLOT(deleteApplication()));
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


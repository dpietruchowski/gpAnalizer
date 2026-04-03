#include "ui/mainwindow.h"
#include "ui/settings.h"
#include "ui_settings.h"
#include "myapplication.h"
#include <QApplication>
#include <cstdio>
#include <stdlib.h>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include "exceptions.h"

int main(int argc, char *argv[]) try
{
    srand(time(NULL));

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
catch (const GpException& ex)
{
    std::cout << ex.what() << std::endl;
}
catch (std::exception & e)
{
    std::cout << e.what() << std::endl;
}
catch (...)
{
    std::cout << "Inny wyjatek" << std::endl;
}


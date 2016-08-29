#include "ui/applicationtab.h"
#include "ui/settings.h"
#include "ui/mainwindow.h"
#include "ui_settings.h"
#include "myapplication.h"
#include <QApplication>
#include <cstdio>
#include <stdlib.h>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int main(int argc, char *argv[]) try
{
    srand(time(NULL));

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
catch (std::string & ex)
{
    cout << ex << endl;
}
catch (std::exception & e)
{
    cout << e.what() << endl;
}
catch (const char *exc)
{
    cout << exc << endl;
}

catch (...)
{
    cout<< "Inny wyjatek" << endl;
}


//int main(int argc, char* argv[])
//{
//    try
//    {


//        Parser parser;
//        TerminalNode::getTerminalSet().setTerminal(MatPtr(new Mat(imread(argv[2], 0))));
//        TreePtr tree = parser.parse(argv[1]);

//        for(int i = 0; i < 31; ++i)
//        {
//            TreePtr subtree = tree->cloneSubtree(i, 0);
//            Mat result = subtree->run();
////            threshold(result, result, 170,255,0);

//            //imshow("result", result);
//            string saveName(argv[2]);
//            saveName = saveName.substr(0, saveName.size() - 4);
//            saveName += "_" + to_string(i) + "_out.png";
//            imwrite(saveName, result);
//        }


//    } catch(std::string& exc)
//    {
//        cout<<exc<<endl;
//    }

//    return 0;
//}

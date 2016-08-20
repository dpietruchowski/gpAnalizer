#include "mainwindow.h"
#include <QApplication>
#include <cstdio>
#include <stdlib.h>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    srand(time(NULL));

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}


//int main(int argc, char* argv[])
//{
//    try
//    {
//        Parser parser;
//        TerminalNode::getTerminalSet().setTerminal(MatPtr(new Mat(imread(argv[2], 0))));
//        TreePtr tree = parser.parse(argv[1]);

//        Mat result = tree->run();
//        threshold(result, result, 125,255,0);

//        //imshow("result", result);
//        string saveName(argv[2]);
//        saveName = saveName.substr(0, saveName.size() - 4);
//        saveName += "_out.png";
//        imwrite(saveName, result);
//        waitKey();
//    } catch(std::string& exc)
//    {
//        cout<<exc<<endl;
//    }

//    return 0;
//}

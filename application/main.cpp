#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

#include <cstdio>
#include <stdlib.h>
#include <vector>
#include "tree.h"
#include "application.h"


int main()
{
    string katalog = "images";

    string command = "rm -r  ";
    command += katalog;
    const char *cstr = command.c_str();
    system(cstr);
    command = "mkdir ";
    command += katalog;
    cstr = command.c_str();
    system(cstr);


    srand(time(NULL));
    application app;
    app.setKatalog(katalog);


    app.start();
/*
    functionSet functions;
    terminalSet terminals;

    Mat inputImage = imread("inwariancje.png");
    if( !inputImage.data )
    {
        string exception = "Could not open or find the input image";
        throw exception;
    }
    terminals.setInputImage( Mat_ptr(new Mat(inputImage)) );

    tree_ptr program (new tree(5));

    pair<funcId, func> p;
    funcId ff;
    ff = make_pair(2,"bitwiseAnd");
    p = make_pair(ff, functionSet::bitwiseAnd);

    node_ptr root (new funcNode(p));

    pair<funcId, morpho> m;
    ff = make_pair(1,"morphologyOperation");
    m = make_pair(ff, functionSet::morphologyOperation);

    morphologyParameters morph1 = {3, 2, 1, 4, 14};
    morphologyParameters morph3 = {4, 3, 2, 5, 11};
    morphologyParameters morph4 = {4, 1, 1, 3, 31};
    morphologyParameters morph2 = {5, 3, 1, 6, 2};

    node_ptr term1 (new termNode(terminals.getRandomImage()));
    node_ptr term2 (new termNode(terminals.getRandomImage()));

    node_ptr nod1 (new morphoNode(m, morph1));
    node_ptr nod3 (new morphoNode(m, morph3));
    node_ptr nod4 (new morphoNode(m, morph4));
    node_ptr nod2 (new morphoNode(m, morph2));

    nod4->addChild(move(term1));
    nod3->addChild(move(nod4));
    nod1->addChild(move(nod3));

    nod2->addChild(move(term2));

    root->addChild(move(nod1));
    root->addChild(move(nod2));

    program->setRoot(move(root));

    Mat result = program->run();

    imshow("res", result);
    waitKey();
*/
    return 0;
}


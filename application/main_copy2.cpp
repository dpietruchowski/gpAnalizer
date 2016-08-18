
/*#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

#include <cstdio>
#include <stdlib.h>
#include <vector>
//#include "tree.h"

/*int main()
{
    functionSet functions;
    functions.showFunctionsAddress();

    tree* individual= new tree(2);
    individual->initialize(FULL_INIT);

    int result;

    individual->show();

    result = individual->run();
    cout<<result<<endl;

    //individual->run();

}

int main()
{
    //cout << "Hello World!" << endl;

    Mat inputImage1 = imread("imgwejsciowy1.png");
    Mat inputImage2 = imread("imgwyjsciowy1.png");

    Mat outputImage1,outputImage2,outputImage;

   // morphologyEx(inputImage1,outputImage,MORPH_DILATE,getStructuringElement(MORPH_CROSS, Size(7,7)));


    for (int i=0; i<200; i++)
    {
        int size, morphType, elementType;
        Mat term1,term2;
///////////
        size = 1 + rand() % 100;
        morphType = rand() % 5;
        elementType = rand() % 3;
        Mat element = getStructuringElement(elementType, Size(size,size));
        morphologyEx(inputImage1,term1,morphType,element);

        size = 1 + rand() % 10;
        morphType = rand() % 5;
        elementType = rand() % 3;
        element = getStructuringElement(elementType, Size(size,size));
        morphologyEx(inputImage1,term2,morphType,element);
        max(term1,term2,outputImage1);
//////////
///////////
        size = 1 + rand() % 100;
        morphType = rand() % 5;
        elementType = rand() % 3;
        element = getStructuringElement(elementType, Size(size,size));
        morphologyEx(inputImage1,term1,morphType,element);

        size = 1 + rand() % 10;
        morphType = rand() % 5;
        elementType = rand() % 3;
        element = getStructuringElement(elementType, Size(size,size));
        morphologyEx(inputImage1,term2,morphType,element);
        bitwise_or(term1,term2,outputImage2);
//////////

        absdiff(outputImage1,outputImage2,outputImage);
        Mat result;
        compare(outputImage,inputImage2,result,CMP_EQ);

        cvtColor(result,result, CV_BGR2GRAY);
        int spr = countNonZero(result);

        //morphologyEx(outputImage,outputImage,morphType,element);

        //imshow("output", outputImage);
        //cout<<size<<" "<<morphType<<" "<<elementType<<endl;
        cout<<spr<<endl;
        if (spr > 230000)
        {
            imshow("output", outputImage);
            imshow("wyjsciowy", inputImage2);
            imshow("result", result);
            waitKey();
        }
        //waitKey();
    }

    return 0;
}**/


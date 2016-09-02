#include "hamming.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;

Fitness *Hamming::create(const cv::Mat &referenceImage)
{
    return new Hamming(referenceImage);
}

Hamming::Hamming(string referenceImageName):
    Fitness(referenceImageName)
{
}

Hamming::Hamming(const cv::Mat &referenceImage):
    Fitness(referenceImage)
{

}

int Hamming::fitness(cv::Mat &A, cv::Mat &B) const
{
    cv::Mat C;

    cv::bitwise_not(A, A);
    cv::bitwise_not(B, B);
    cv::bitwise_xor(A, B, C);
    int measure = cv::countNonZero(C); //czarne pixele
    if (countNonZero(A)  < countNonZero(B))
        measure += countNonZero(C)*10;
    else measure = countNonZero(C);


    return measure;
}

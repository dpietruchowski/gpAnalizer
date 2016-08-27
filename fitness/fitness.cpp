#include "fitness.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <numeric>

using namespace std;

Fitness::Fitness()
{

}

Fitness::Fitness(string referenceImageName)
{
    referenceImage_ = cv::imread(referenceImageName);
}

Fitness::Fitness(const cv::Mat &referenceImage):
    referenceImage_(referenceImage)
{

}

int Fitness::measure(const cv::Mat &image) const
{
    int measure = 1000000;//?
    cv::Mat C, D;
    cv::threshold(image, C, 125, 255, 0);
    threshold(referenceImage_, D, 125, 255, 0);
    cv::extractChannel(C, C, 0);
    cv::extractChannel(D, D, 0);

    measure = fitness(C, D);

    return measure;
}

void Fitness::setReferenceImage(std::string referenceImage)
{

    referenceImage_ = cv::imread(referenceImage);
    if(referenceImage_.empty())
        throw "Terminal pusty";
}

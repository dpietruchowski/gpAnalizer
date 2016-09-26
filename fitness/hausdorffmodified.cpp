#include "hausdorffmodified.h"

#include <algorithm>

using namespace std;

Fitness *HausdorffModified::create(const cv::Mat &referenceImage)
{
    return new HausdorffModified(referenceImage);
}

HausdorffModified::HausdorffModified(std::string referenceImageName):
    Hausdorff(referenceImageName)
{
}

HausdorffModified::HausdorffModified(const cv::Mat &referenceImage):
    Hausdorff(referenceImage)
{
}

void HausdorffModified::randomizePoints(std::vector<cv::Point> &a, std::vector<cv::Point> &b) const
{
    randomizePoints(a);
    randomizePoints(b);
}

void HausdorffModified::randomizePoints(std::vector<cv::Point> &a) const
{
    random_shuffle(a.begin(), a.end());
}

#include "hausdorffsmall.h"
#include <opencv2/imgproc/imgproc.hpp>

Fitness *HausdorffSmall::create(const cv::Mat& referenceImage)
{
    return new HausdorffSmall(referenceImage);
}

HausdorffSmall::HausdorffSmall(std::string referenceImageName):
    Hausdorff(referenceImageName),
    width_(220), height_(100)
{
}

HausdorffSmall::HausdorffSmall(std::string referenceImageName,
                             int width, int height):
    Hausdorff(referenceImageName),
    width_(width), height_(height)
{
}

HausdorffSmall::HausdorffSmall(const cv::Mat &referenceImage):
    Hausdorff(referenceImage),
    width_(220), height_(100)
{

}

void HausdorffSmall::transformImages(cv::Mat &A, cv::Mat &B) const
{
    cv::resize(A, A, cv::Size(width_,height_));
    cv::resize(B, B, cv::Size(width_,height_));
}

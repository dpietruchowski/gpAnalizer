#include "hausdorffcanny.h"
#include <opencv2/imgproc/imgproc.hpp>

Fitness *HausdorffCanny::create(const cv::Mat& referenceImage)
{
    return new HausdorffCanny(referenceImage);
}

HausdorffCanny::HausdorffCanny(std::string referenceImageName):
    Hausdorff(referenceImageName)
{
}

HausdorffCanny::HausdorffCanny(const cv::Mat &referenceImage):
    Hausdorff(referenceImage)
{

}


void HausdorffCanny::transformImages(cv::Mat &A, cv::Mat &B) const
{
    cv::Canny(A, A, 20, 20*30);
    cv::Canny(B, B, 20, 20*30);
}

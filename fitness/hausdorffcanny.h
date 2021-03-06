#ifndef HAUSDORFFCANNY_H
#define HAUSDORFFCANNY_H

#include "hausdorff.h"

class HausdorffCanny: public Hausdorff
{
public:
    static Fitness* create(const cv::Mat& referenceImage);
    HausdorffCanny(std::string referenceImageName);
    HausdorffCanny(const cv::Mat& referenceImage);

private:
    virtual void transformImages(cv::Mat& A, cv::Mat& B) const;
};

#endif // HAUSDORFFCANNY_H

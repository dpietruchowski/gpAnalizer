#ifndef HAUSDORFFMODIFIED_H
#define HAUSDORFFMODIFIED_H

#include "hausdorff.h"

class HausdorffModified : public Hausdorff
{
public:
    static Fitness* create(const cv::Mat& referenceImage);
    HausdorffModified(std::string referenceImageName);
    HausdorffModified(const cv::Mat& referenceImage);

private:
    virtual void randomizePoints(std::vector<cv::Point> &a,
                                 std::vector<cv::Point> &b) const;
    virtual void randomizePoints(std::vector<cv::Point> &a) const;
};

#endif // HAUSDORFFMODIFIED_H

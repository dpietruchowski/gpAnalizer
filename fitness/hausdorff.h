#ifndef HAUSDORFF_H
#define HAUSDORFF_H

#include "fitness.h"

class Hausdorff: public Fitness
{
public:
    static Fitness* create(const cv::Mat& referenceImage);
    Hausdorff(std::string referenceImageName);
    Hausdorff(const cv::Mat& referenceImage);

private:
    virtual int fitness(cv::Mat &A, cv::Mat &B) const;
    virtual void transformImages(cv::Mat &A,
                            cv::Mat &B) const;
    virtual void randomizePoints(std::vector<cv::Point> &a,
                                 std::vector<cv::Point> &b) const;
    int distance(const std::vector<cv::Point> &a,
                 const std::vector<cv::Point> &b) const;
    int distance(const cv::Mat& A, const cv::Mat& B) const;
};

#endif // HAUSDORFF_H

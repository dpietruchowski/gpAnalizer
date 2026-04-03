#include "hausdorff.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

Fitness *Hausdorff::create(const cv::Mat& referenceImage)
{
    return new Hausdorff(referenceImage);
}

Hausdorff::Hausdorff(std::string referenceImageName):
    Fitness(referenceImageName)
{
    maxValue_ = 1000000;
}

Hausdorff::Hausdorff(const cv::Mat &referenceImage):
    Fitness(referenceImage)
{
    maxValue_ = referenceImage.rows*referenceImage.rows;
    maxValue_ += referenceImage.cols*referenceImage.cols;
}

int Hausdorff::fitness(cv::Mat &A, cv::Mat &B) const
{
    if(countNonZero(A) == 0)
    {
        A.at<uchar>(0,0) = 0;
        A.at<uchar>(1,0) = 0;
        A.at<uchar>(1,1) = 0;
        A.at<uchar>(0,1) = 0;
    }

    int distanceAB = distance(A, B);
    int distanceBA = distance(B, A);

    return std::max(distanceAB, distanceBA);
}

void Hausdorff::transformImages(cv::Mat &A, cv::Mat &B) const
{
    cv::Mat sumAB;

    cv::bitwise_or(A, B, sumAB); // A sum B
    cv::bitwise_not(sumAB, sumAB);

    cv::bitwise_or(A, sumAB, A); // roznica A - A sum B

    cv::bitwise_not(A, A);
    cv::bitwise_not(B, B);
}

void Hausdorff::randomizePoints(std::vector<cv::Point> &,
                                std::vector<cv::Point> &) const
{
    // do nothing
}

int Hausdorff::distance(const std::vector<cv::Point> &a,
                        const std::vector<cv::Point> &b) const
{
    int maxDistance = 0;
    for (size_t i = 0; i < a.size(); i++)
    {
        int minAB = maxValue_;
        for (size_t j = 0; j < b.size(); j++)
        {
           int dx = abs(a[i].x - b[j].x);
           int dy = abs(a[i].y - b[j].y);
           int tmpDst = dx*dx + dy*dy;

           if (tmpDst < minAB)
               minAB = tmpDst;

           if (tmpDst < maxDistance)
               break;

        }
        if(minAB > maxDistance)
            maxDistance = minAB;
    }

    return maxDistance;
}

int Hausdorff::distance(const cv::Mat &A, const cv::Mat &B) const
{
    cv::Mat C = A.clone();
    cv::Mat D = B.clone();

    transformImages(C, D);

    std::vector<cv::Point> a, b;
    cv::findNonZero(C, a);
    cv::findNonZero(D, b);

    randomizePoints(a, b);

    return distance(a, b);
}

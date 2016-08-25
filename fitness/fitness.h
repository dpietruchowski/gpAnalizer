#ifndef MEPFITNESS_H
#define MEPFITNESS_H

#include <memory>
#include <vector>
#include <opencv2/core/core.hpp>

enum FitnessType
{
    HAUSDORFF,
    HAUSDORFF_CANNY,
    HAMMING
};

class Fitness
{
public:
    Fitness(std::string referenceImageName);
    Fitness(const cv::Mat& referenceImage);
    virtual ~Fitness() {}
public:
    int measure(const cv::Mat& image) const;

    void setReferenceImage(std::string referenceImage);

private:
    virtual int fitness(cv::Mat &A, cv::Mat &B) const = 0;
    Fitness();

private:
    cv::Mat referenceImage_;
};

#endif // MEPFITNESS_H

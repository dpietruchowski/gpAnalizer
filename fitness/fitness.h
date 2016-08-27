#ifndef MEPFITNESS_H
#define MEPFITNESS_H

#include <memory>
#include <vector>
#include <opencv2/core/core.hpp>

enum FitnessType
{
    HAMMING,
    HAUSDORFF_MODIFIED,
    HAUSDORFF_CANNY
};

inline FitnessType fitnessTypeFromString(std::string& enm)
{
    if(enm == "HAMMING") return HAMMING;
    if(enm == "HAUSDORFF_MODIFIED") return HAUSDORFF_MODIFIED;
    if(enm == "HAUSDORFF_CANNY") return HAUSDORFF_CANNY;

    throw std::string("Wrong enum");
}

inline std::string fitnessTypeToString(FitnessType type)
{
    if(type == HAMMING) return "HAMMING";
    if(type == HAUSDORFF_MODIFIED) return "HAUSDORFF_MODIFIED";
    if(type == HAUSDORFF_CANNY) return "HAUSDORFF_CANNY";

    throw std::string("Wrong enum");

}

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

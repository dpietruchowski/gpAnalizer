#ifndef FUNCTIONSET_H
#define FUNCTIONSET_H

#include <vector>
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string>
#include <map>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include "typeFunc.h"


typedef std::pair<int, std::string> funcId;
struct morphologyParameters
{
    int size1;
    int size2;
    int elementType;
    int morphType;
    int iteration;
};
typedef int(* func) (const std::vector<cv::Mat>&, cv::Mat&);
typedef int(* morpho) (const std::vector<cv::Mat>&, cv::Mat&, morphologyParameters);


class functionSet
{
private:
    std::map< std::string, std::pair<int,func> > functionsNotAdded;
    std::map< std::string, std::pair<int,morpho> > operationsNotAdded;
    std::map< std::string, std::pair<int,func> > functions;
    std::map< std::string, std::pair<int,morpho> > operations;
public:
    functionSet();
    ~functionSet();
    std::pair<funcId,func> getRandomFunction() const;
    std::pair<funcId,func> getRandomFunction(int argumentsNumber) const;
    std::pair<funcId,morpho> getRandomOperation() const;
    morphologyParameters getRandomMorphParameters() const;
    void clear();
    void addFunction(std::string functionName);
    void addOperation(std::string operationName);
public:
    static int absDiff(const std::vector<cv::Mat>& src, cv::Mat& dst);
    static int addF(const std::vector<cv::Mat>& src, cv::Mat& dst);
    static int bitwiseAnd(const std::vector<cv::Mat>& src, cv::Mat& dst);
    static int bitwiseXor(const std::vector<cv::Mat>& src, cv::Mat& dst);
    static int bitwiseOr(const std::vector<cv::Mat>& src, cv::Mat& dst);
    static int minF(const std::vector<cv::Mat>& src, cv::Mat& dst);
    static int maxF(const std::vector<cv::Mat>& src, cv::Mat& dst);
    static int bitwiseNot(const std::vector<cv::Mat>& src, cv::Mat& dst);
    static int thresHold(const std::vector<Mat>& src, Mat& dst);

    static int morphologyOperation(const std::vector<cv::Mat>& src, cv::Mat& dst, morphologyParameters morph);
};

#endif // FUNCTIONSET_H

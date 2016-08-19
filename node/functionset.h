#ifndef FUNCTIONSET_H
#define FUNCTIONSET_H

#include <vector>
#include <string>
#include <map>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

typedef int(* FunctionPtr) (const std::vector<cv::Mat>&, cv::Mat&);
struct FunctionId
{
    std::string name;
    int nArguments;
};

class FunctionSet
{
public:
    FunctionSet();
    ~FunctionSet();
    std::pair<FunctionId,FunctionPtr> getRandomFunction() const;
    std::pair<FunctionId,FunctionPtr> getRandomFunction(int argumentsNumber) const;
    void addFunction(std::string name);
    void deleteFunction(std::string name);
    void addAllFunctions();
    void deleteAllFunctions();
public:
    static int bitwiseAnd(const std::vector<cv::Mat>& src, cv::Mat& dst);
    static int bitwiseXor(const std::vector<cv::Mat>& src, cv::Mat& dst);
    static int bitwiseOr(const std::vector<cv::Mat>& src, cv::Mat& dst);
    static int bitwiseNot(const std::vector<cv::Mat>& src, cv::Mat& dst);
    static int absDiff(const std::vector<cv::Mat>& src, cv::Mat& dst);
    static int addF(const std::vector<cv::Mat>& src, cv::Mat& dst);
    static int minF(const std::vector<cv::Mat>& src, cv::Mat& dst);
    static int maxF(const std::vector<cv::Mat>& src, cv::Mat& dst);

private:
    typedef std::map<std::string, std::pair<int,FunctionPtr>> Functions;
private:
    std::pair<FunctionId,FunctionPtr> conversion(Functions::const_iterator it) const;
private:
    Functions functionsNotAdded_;
    Functions functions_;
};

#endif // FUNCTIONSET_H

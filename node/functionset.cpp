#include "functionset.h"
#include "../exceptions.h"

FunctionSet::FunctionSet()
{
    std::pair<int,FunctionPtr> f;
    std::pair<std::string, std::pair<int,FunctionPtr>> p;

    f = std::make_pair(2, FunctionSet::bitwiseAnd);
    p = std::make_pair("bitwiseAnd", f);
    functionsNotAdded_.insert(p);
    f = std::make_pair(2, FunctionSet::bitwiseXor);
    p = std::make_pair("bitwiseXor", f);
    functionsNotAdded_.insert(p);
    f = std::make_pair(2, FunctionSet::bitwiseOr);
    p = std::make_pair("bitwiseOr", f);
    functionsNotAdded_.insert(p);
    f = std::make_pair(1, FunctionSet::bitwiseNot);
    p = std::make_pair("bitwiseNot", f);
    functionsNotAdded_.insert(p);
    f = std::make_pair(2, FunctionSet::diff);
    p = std::make_pair("diff", f);
    functionsNotAdded_.insert(p);
    f = std::make_pair(2, FunctionSet::recall);
    p = std::make_pair("recall", f);
    functionsNotAdded_.insert(p);

    addAllFunctions();
}

FunctionSet::~FunctionSet()
{
}

std::pair<FunctionId, FunctionPtr> FunctionSet::getRandomFunction() const
{
    Functions::const_iterator it = functions_.begin();
    std::advance(it, std::rand() % functions_.size() );

    return conversion(it);
}

std::pair<FunctionId, FunctionPtr> FunctionSet::getRandomFunction(int argumentsNumber) const
{
    Functions argFunctions;
    Functions::const_iterator it = functions_.begin();
        //Rewrite funciton with the same number of arguments as argumentsNumber
    for(auto it = functions_.begin(); it != functions_.end();)
    {
        if(it->second.first == argumentsNumber)
            argFunctions.insert(std::make_pair(it->first,it->second));
        ++it;
    }

    if (argFunctions.size() < 1)
    {
        throw InvalidArgumentException("Nie ma zadnej funkcji o takiej liczbie argumentow");
    }

    it = argFunctions.begin();
    std::advance(it, std::rand() % argFunctions.size() );

    return conversion(it);
}

std::pair<FunctionId, FunctionPtr> FunctionSet::getFunction(std::string name) const
{
    Functions::const_iterator it = functions_.find(name);

    if (it == functions_.end())
    {
        throw InvalidArgumentException("Nie ma zadnej funkcji o takiej nazwie lub nie jest dodana");
    }

    return this->conversion(it);
}

void FunctionSet::addFunction(std::string name)
{
    Functions::iterator it;
    it = functionsNotAdded_.find(name);
    if(it == functionsNotAdded_.end())
        throw InvalidArgumentException("Nie ma takiej funkcji");
    functions_.insert(make_pair(it->first, it->second));
}

void FunctionSet::deleteFunction(std::string name)
{
    functions_.erase(functions_.find(name));
}

void FunctionSet::addAllFunctions()
{
    deleteAllFunctions();
    for(const auto& it : functionsNotAdded_)
    {
        functions_.insert(make_pair(it.first, it.second));
    }
}

void FunctionSet::deleteAllFunctions()
{
    functions_.clear();
}

std::pair<FunctionId, FunctionPtr> FunctionSet::conversion(Functions::const_iterator it) const
{
    std::string name = it->first;
    int argumentsNumber = it->second.first;
    FunctionPtr function = it->second.second;
    FunctionId id = {name, argumentsNumber};

    return std::make_pair(id,function);
}

int FunctionSet::absDiff(const std::vector<cv::Mat>& src, cv::Mat& dst)
{
    if (src.size() != 2)
    {
        throw InvalidArgumentException("Zla liczba argumentow");
    }
    const cv::Mat& src1 = src[0];
    const cv::Mat& src2 = src[1];
    cv::absdiff(src1,src2,dst);
    return 2;
}

int FunctionSet::addF(const std::vector<cv::Mat>& src, cv::Mat& dst)
{
    if (src.size() != 2)
    {
        throw InvalidArgumentException("Zla liczba argumentow");
    }
    const cv::Mat& src1 = src[0];
    const cv::Mat& src2 = src[1];
    cv::add(src1,src2,dst);
    return 2;
}

int FunctionSet::bitwiseAnd(const std::vector<cv::Mat>& src, cv::Mat& dst)
{
    if (src.size() != 2)
    {
        throw InvalidArgumentException("Zla liczba argumentow");
    }
    const cv::Mat& src1 = src[0];
    const cv::Mat& src2 = src[1];
    cv::bitwise_and(src1,src2,dst);
    return 2;
}

int FunctionSet::bitwiseXor(const std::vector<cv::Mat>& src, cv::Mat& dst)
{
    if (src.size() != 2)
    {
        throw InvalidArgumentException("Zla liczba argumentow");
    }
    const cv::Mat& src1 = src[0];
    const cv::Mat& src2 = src[1];
    cv::bitwise_xor(src1,src2,dst);
    return 2;
}

int FunctionSet::bitwiseOr(const std::vector<cv::Mat>& src, cv::Mat& dst)
{
    if (src.size() != 2)
    {
        throw InvalidArgumentException("Zla liczba argumentow");
    }
    const cv::Mat& src1 = src[0];
    const cv::Mat& src2 = src[1];
    cv::bitwise_or(src1,src2,dst);
    return 2;
}

int FunctionSet::bitwiseNot(const std::vector<cv::Mat>& src, cv::Mat& dst )
{
    if (src.size() != 1)
    {
        throw InvalidArgumentException("Zla liczba argumentow");
    }
    const cv::Mat& src1 = src[0];
    cv::bitwise_not(src1,dst);
    return 1;
}

int FunctionSet::maxF(const std::vector<cv::Mat>& src, cv::Mat& dst)
{
    if (src.size() != 2)
    {
        throw InvalidArgumentException("Zla liczba argumentow");
    }
    const cv::Mat& src1 = src[0];
    const cv::Mat& src2 = src[1];
    cv::max(src1,src2,dst);
    return 2;
}

int FunctionSet::diff(const std::vector<cv::Mat> &src, cv::Mat &dst)
{
    if (src.size() != 2)
    {
        throw InvalidArgumentException("Zla liczba argumentow");
    }
    const cv::Mat& src1 = src[0];
    const cv::Mat& src2 = src[1];

    cv::Mat sumAB;

    cv::bitwise_or(src1, src2, sumAB); // A sum B
    cv::bitwise_not(sumAB, sumAB);

    cv::bitwise_or(src1, sumAB, dst); // roznica A - A sum B

    return 2;
}

int FunctionSet::recall(const std::vector<cv::Mat> &src, cv::Mat &dst)
{
    if (src.size() != 2)
    {
        throw InvalidArgumentException("Zla liczba argumentow");
    }
    const cv::Mat& src1 = src[0];
    const cv::Mat& src2 = src[1];
    reconstruction(src1,src2,dst);
    return 2;
}

int FunctionSet::fillHoles(const std::vector<cv::Mat> &src, cv::Mat &dst)
{
    if (src.size() != 1)
    {
        throw InvalidArgumentException("Zla liczba argumentow");
    }
    const cv::Mat& src1 = src[0];

    cv::Mat mask;
    mask = cv::Mat(src1.rows, src1.cols, CV_8U);
    cv::Point rook_points[1][20];
    rook_points[0][0] = cv::Point( 0, 0 );
    rook_points[0][1] = cv::Point( mask.cols, 0 );
    rook_points[0][2] = cv::Point( mask.cols, mask.rows );
    rook_points[0][3] = cv::Point( 0, mask.rows );
    const cv::Point* ppt[1] = { rook_points[0] };
    int npt[] = { 4 };
    cv::fillPoly(mask,ppt,npt,1, 255);
    cv::rectangle(mask, cv::Point(0,0), cv::Point(mask.cols-1, mask.rows-1), 0, 2);

    cv::Mat img = src1.clone();
    cv::bitwise_not(img, img);

    reconstruction(mask, img, dst);
    cv::bitwise_not(dst,dst);

    return 1;
}

int FunctionSet::borderConnected(const std::vector<cv::Mat> &src, cv::Mat &dst)
{
    if (src.size() != 1)
    {
        throw InvalidArgumentException("Zla liczba argumentow");
    }
    const cv::Mat& src1 = src[0];

    cv::Mat mask;
    mask = cv::Mat(src1.rows, src1.cols, CV_8U);
    cv::Point rook_points[1][20];
    rook_points[0][0] = cv::Point( 0, 0 );
    rook_points[0][1] = cv::Point( mask.cols, 0 );
    rook_points[0][2] = cv::Point( mask.cols, mask.rows );
    rook_points[0][3] = cv::Point( 0, mask.rows );
    const cv::Point* ppt[1] = { rook_points[0] };
    int npt[] = { 4 };
    cv::fillPoly(mask,ppt,npt,1, 255);
    cv::rectangle(mask, cv::Point(0,0), cv::Point(mask.cols-1, mask.rows-1), 0, 2);

    cv::Mat img = src1.clone();

    reconstruction(mask, img, dst);

    return 1;
}

int FunctionSet::borderDisconnected(const std::vector<cv::Mat> &src, cv::Mat &dst)
{
    if (src.size() != 1)
    {
        throw InvalidArgumentException("Zla liczba argumentow");
    }
    const cv::Mat& src1 = src[0];
    cv::Mat out;
    FunctionSet::borderConnected(src, out);
    std::vector<cv::Mat> imgs;
    imgs.push_back(src1);
    imgs.push_back(out);
    FunctionSet::diff(imgs, dst);

    return 1;
}

void FunctionSet::reconstruction(const cv::Mat &src, const cv::Mat &mask, cv::Mat &dst)
{
    cv::Mat invSrc;
    cv::bitwise_not(src, invSrc);
    cv::Mat invMask;
    cv::bitwise_not(mask, invMask);
    cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5));

    cv::morphologyEx(invSrc, dst, cv::MORPH_DILATE, element,cv::Point(-1,-1), 1);
    cv::bitwise_and(dst, invMask, dst);
    cv::Mat prev = dst.clone();
    cv::morphologyEx(dst, dst, cv::MORPH_DILATE, element,cv::Point(-1,-1), 1);
    cv::bitwise_and(dst, invMask, dst);
    cv::Mat tmp;
    cv::bitwise_xor(dst, prev, tmp);
    int pixels = cv::countNonZero(tmp);
    while(pixels != 0)
    {
        prev = dst.clone();
        cv::morphologyEx(dst, dst, cv::MORPH_DILATE, element,cv::Point(-1,-1), 1);
        cv::bitwise_and(dst, invMask, dst);
        cv::bitwise_xor(dst, prev, tmp);
        pixels = cv::countNonZero(tmp);
    }

    cv::bitwise_not(dst, dst);
}

int FunctionSet::minF(const std::vector<cv::Mat>& src, cv::Mat& dst)
{
    if (src.size() != 2)
    {
        throw InvalidArgumentException("Zla liczba argumentow");
    }
    const cv::Mat& src1 = src[0];
    const cv::Mat& src2 = src[1];
    cv::min(src1,src2,dst);
    return 2;
}

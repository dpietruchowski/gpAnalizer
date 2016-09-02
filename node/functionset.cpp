#include "functionset.h"

using namespace std;
using namespace cv;

FunctionSet::FunctionSet()
{
    pair<int,FunctionPtr> f;
    pair<string, pair<int,FunctionPtr>> p;

    f = make_pair(2, FunctionSet::bitwiseAnd);
    p = make_pair("bitwiseAnd", f);
    functionsNotAdded_.insert(p);
    f = make_pair(2, FunctionSet::bitwiseXor);
    p = make_pair("bitwiseXor", f);
    functionsNotAdded_.insert(p);
    f = make_pair(2, FunctionSet::bitwiseOr);
    p = make_pair("bitwiseOr", f);
    functionsNotAdded_.insert(p);
    f = make_pair(1, FunctionSet::bitwiseNot);
    p = make_pair("bitwiseNot", f);
    functionsNotAdded_.insert(p);
    f = make_pair(2, FunctionSet::diff);
    p = make_pair("diff", f);
    functionsNotAdded_.insert(p);
    f = make_pair(2, FunctionSet::recall);
    p = make_pair("recall", f);
    functionsNotAdded_.insert(p);
    f = make_pair(1, FunctionSet::fillHoles);
    p = make_pair("fillHoles", f);
    functionsNotAdded_.insert(p);/*
    f = make_pair(1, FunctionSet::borderConnected);
    p = make_pair("borderConnected", f);
    functionsNotAdded_.insert(p);
    f = make_pair(1, FunctionSet::borderDisconnected);
    p = make_pair("borderDisconnected", f);
    functionsNotAdded_.insert(p);*/

    f = make_pair(2, FunctionSet::absDiff);
    p = make_pair("absDiff", f);
//    functionsNotAdded_.insert(p);
    f = make_pair(2, FunctionSet::addF);
    p = make_pair("addF", f);
//    functionsNotAdded_.insert(p);           //   <-----------------nie dodana
    f = make_pair(2, FunctionSet::minF);
    p = make_pair("minF", f);
//    functionsNotAdded_.insert(p);          //    <-----------------nie dodana
    f = make_pair(2, FunctionSet::maxF);
    p = make_pair("maxF", f);
//    functionsNotAdded_.insert(p);     //         <-----------------nie dodana

    addAllFunctions();
}

FunctionSet::~FunctionSet()
{
}

std::pair<FunctionId, FunctionPtr> FunctionSet::getRandomFunction() const
{
    Functions::const_iterator it = functions_.begin();
    std::advance(it, rand() % functions_.size() );

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
            argFunctions.insert(make_pair(it->first,it->second));
        ++it;
    }

    if (argFunctions.size() < 1)
    {
        string exception = "Nie ma zadnej funkcji o takiej liczbie argumentow";
        throw exception;
    }

    it = argFunctions.begin();
    std::advance(it, rand() % argFunctions.size() );

    return conversion(it);
}

std::pair<FunctionId, FunctionPtr> FunctionSet::getFunction(string name) const
{
    Functions::const_iterator it = functions_.find(name);

    if (it == functions_.end())
    {
        string exception = "Nie ma zadnej funkcji o takiej nazwie"
                           "lub nie jest dodana";
        throw exception;
    }

    return this->conversion(it);
}

void FunctionSet::addFunction(string name)
{
    Functions::iterator it;
    it = functionsNotAdded_.find(name);
    if(it == functionsNotAdded_.end())
        throw string("Nie ma takiej funkcji");
    functions_.insert(make_pair(it->first, it->second));
}

void FunctionSet::deleteFunction(string name)
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
    string name = it->first;
    int argumentsNumber = it->second.first;
    FunctionPtr function = it->second.second;
    FunctionId id = {name, argumentsNumber};

    return make_pair(id,function);
}

int FunctionSet::absDiff(const vector<Mat>& src, Mat& dst)
{
    if (src.size() != 2)
    {
        string exception = "Zla liczba argumentow";
        throw exception;
    }
    const Mat& src1 = src[0];
    const Mat& src2 = src[1];
    absdiff(src1,src2,dst);
    return 2;
}

int FunctionSet::addF(const vector<Mat>& src, Mat& dst)
{
    if (src.size() != 2)
    {
        string exception = "Zla liczba argumentow";
        throw exception;
    }
    const Mat& src1 = src[0];
    const Mat& src2 = src[1];
    add(src1,src2,dst);
    return 2;
}

int FunctionSet::bitwiseAnd(const vector<Mat>& src, Mat& dst)
{
    if (src.size() != 2)
    {
        string exception = "Zla liczba argumentow";
        throw exception;
    }
    const Mat& src1 = src[0];
    const Mat& src2 = src[1];
    bitwise_and(src1,src2,dst);
    return 2;
}

int FunctionSet::bitwiseXor(const vector<Mat>& src, Mat& dst)
{
    if (src.size() != 2)
    {
        string exception = "Zla liczba argumentow";
        throw exception;
    }
    const Mat& src1 = src[0];
    const Mat& src2 = src[1];
    bitwise_xor(src1,src2,dst);
    return 2;
}

int FunctionSet::bitwiseOr(const vector<Mat>& src, Mat& dst)
{
    if (src.size() != 2)
    {
        string exception = "Zla liczba argumentow";
        throw exception;
    }
    const Mat& src1 = src[0];
    const Mat& src2 = src[1];
    bitwise_or(src1,src2,dst);
    return 2;
}

int FunctionSet::bitwiseNot(const vector<Mat>& src, Mat& dst )
{
    if (src.size() != 1)
    {
        string exception = "Zla liczba argumentow";
        throw exception;
    }
    const Mat& src1 = src[0];
    bitwise_not(src1,dst);
    return 1;
}

int FunctionSet::maxF(const vector<Mat>& src, Mat& dst)
{
    if (src.size() != 2)
    {
        string exception = "Zla liczba argumentow";
        throw exception;
    }
    const Mat& src1 = src[0];
    const Mat& src2 = src[1];
    max(src1,src2,dst);
    return 2;
}

int FunctionSet::diff(const std::vector<Mat> &src, Mat &dst)
{
    if (src.size() != 2)
    {
        string exception = "Zla liczba argumentow";
        throw exception;
    }
    const Mat& src1 = src[0];
    const Mat& src2 = src[1];

    Mat sumAB;

    bitwise_or(src1, src2, sumAB); // A sum B
    bitwise_not(sumAB, sumAB);

    bitwise_or(src1, sumAB, dst); // roznica A - A sum B

    return 2;
}

int FunctionSet::recall(const std::vector<Mat> &src, Mat &dst)
{
    if (src.size() != 2)
    {
        string exception = "Zla liczba argumentow";
        throw exception;
    }
    const Mat& src1 = src[0];
    const Mat& src2 = src[1];
    reconstruction(src1,src2,dst);
    return 2;
}

int FunctionSet::fillHoles(const std::vector<Mat> &src, Mat &dst)
{
    if (src.size() != 1)
    {
        string exception = "Zla liczba argumentow";
        throw exception;
    }
    const Mat& src1 = src[0];

    Mat mask;
    mask = Mat(src1.rows, src1.cols, CV_8U);
    Point rook_points[1][20];
    rook_points[0][0] = Point( 0, 0 );
    rook_points[0][1] = Point( mask.cols, 0 );
    rook_points[0][2] = Point( mask.cols, mask.rows );
    rook_points[0][3] = Point( 0, mask.rows );
    const Point* ppt[1] = { rook_points[0] };
    int npt[] = { 4 };
    fillPoly(mask,ppt,npt,1, 255);
    rectangle(mask, Point(0,0), Point(mask.cols-1, mask.rows-1), 0, 2);

    Mat img = src1.clone();
    bitwise_not(img, img);

    reconstruction(mask, img, dst);
    bitwise_not(dst,dst);

    return 1;
}

int FunctionSet::borderConnected(const std::vector<Mat> &src, Mat &dst)
{
    if (src.size() != 1)
    {
        string exception = "Zla liczba argumentow";
        throw exception;
    }
    const Mat& src1 = src[0];

    Mat mask;
    mask = Mat(src1.rows, src1.cols, CV_8U);
    Point rook_points[1][20];
    rook_points[0][0] = Point( 0, 0 );
    rook_points[0][1] = Point( mask.cols, 0 );
    rook_points[0][2] = Point( mask.cols, mask.rows );
    rook_points[0][3] = Point( 0, mask.rows );
    const Point* ppt[1] = { rook_points[0] };
    int npt[] = { 4 };
    fillPoly(mask,ppt,npt,1, 255);
    rectangle(mask, Point(0,0), Point(mask.cols-1, mask.rows-1), 0, 2);

    Mat img = src1.clone();

    reconstruction(mask, img, dst);

    return 1;
}

int FunctionSet::borderDisconnected(const std::vector<Mat> &src, Mat &dst)
{
    if (src.size() != 1)
    {
        string exception = "Zla liczba argumentow";
        throw exception;
    }
    const Mat& src1 = src[0];
    Mat out;
    FunctionSet::borderConnected(src, out);
    vector<Mat> imgs;
    imgs.push_back(src1);
    imgs.push_back(out);
    FunctionSet::diff(imgs, dst);

    return 1;
}

void FunctionSet::reconstruction(const Mat &src, const Mat &mask, Mat &dst)
{
    Mat invSrc;
    bitwise_not(src, invSrc);
    Mat invMask;
    bitwise_not(mask, invMask);
    Mat element = getStructuringElement(MORPH_ELLIPSE, Size(5,5));

    morphologyEx(invSrc, dst, MORPH_DILATE, element,Point(-1,-1), 1);
    bitwise_and(dst, invMask, dst);
    Mat prev = dst.clone();
    morphologyEx(dst, dst, MORPH_DILATE, element,Point(-1,-1), 1);
    bitwise_and(dst, invMask, dst);
    Mat tmp;
    bitwise_xor(dst, prev, tmp);
    int pixels = countNonZero(tmp);
    while(pixels != 0)
    {
        prev = dst.clone();
        morphologyEx(dst, dst, MORPH_DILATE, element,Point(-1,-1), 1);
        bitwise_and(dst, invMask, dst);
        bitwise_xor(dst, prev, tmp);
        pixels = countNonZero(tmp);
    }

    bitwise_not(dst, dst);
}

int FunctionSet::minF(const vector<Mat>& src, Mat& dst)
{
    if (src.size() != 2)
    {
        string exception = "Zla liczba argumentow";
        throw exception;
    }
    const Mat& src1 = src[0];
    const Mat& src2 = src[1];
    min(src1,src2,dst);
    return 2;
}

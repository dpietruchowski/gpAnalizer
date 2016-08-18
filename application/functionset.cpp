#include "functionset.h"

using namespace std;
using namespace cv;

functionSet::functionSet()
{
    pair<int,func> f;
    pair< string, pair<int,func> > p;

    f = make_pair(2,functionSet::absDiff);
    p = make_pair("absDiff", f);
    functionsNotAdded.insert(p);
    f = make_pair(2,functionSet::addF);
    p = make_pair("addF", f);
    functionsNotAdded.insert(p);
    f = make_pair(2,functionSet::bitwiseAnd);
    p = make_pair("bitwiseAnd", f);
    functionsNotAdded.insert(p);
    f = make_pair(2,functionSet::bitwiseXor);
    p = make_pair("bitwiseXor", f);
    functionsNotAdded.insert(p);
    f = make_pair(2,functionSet::bitwiseOr);
    p = make_pair("bitwiseOr", f);
    functionsNotAdded.insert(p);
    f = make_pair(2,functionSet::minF);
    p = make_pair("minF", f);
    functionsNotAdded.insert(p);
    f = make_pair(2,functionSet::maxF);
    p = make_pair("maxF", f);
    functionsNotAdded.insert(p);
    f = make_pair(1,functionSet::bitwiseNot);
    p = make_pair("bitwiseNot", f);
    functionsNotAdded.insert(p);
    f = make_pair(1,functionSet::thresHold);
    p = make_pair("threshold", f);
    functionsNotAdded.insert(p);

    pair<int,morpho> o;
    pair< string, pair<int,morpho> > r;
    o = make_pair(1,functionSet::morphologyOperation);
    r = make_pair("morphologyOperation", o);
    operationsNotAdded.insert(r);
}

functionSet::~functionSet()
{
}

int functionSet::absDiff(const vector<Mat>& src, Mat& dst)
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

int functionSet::addF(const vector<Mat>& src, Mat& dst)
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

int functionSet::bitwiseAnd(const vector<Mat>& src, Mat& dst)
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

int functionSet::bitwiseXor(const vector<Mat>& src, Mat& dst)
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

int functionSet::bitwiseOr(const vector<Mat>& src, Mat& dst)
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

int functionSet::maxF(const vector<Mat>& src, Mat& dst)
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

int functionSet::minF(const vector<Mat>& src, Mat& dst)
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

int functionSet::bitwiseNot(const vector<Mat>& src, Mat& dst )
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

int functionSet::thresHold(const vector<Mat>& src, Mat& dst)
{
    if (src.size() != 1)
    {
        string exception = "Zla liczba argumentow";
        throw exception;
    }
    const Mat& src1 = src[0];
    threshold(src1, dst, 125, 255, 0);
    return 1;
}


int functionSet::morphologyOperation(const vector<Mat>& src, Mat& dst, morphologyParameters morph)
{
    if (src.size() != 1)
    {
        string exception = "Zla liczba argumentow";
        throw exception;
    }
    Mat src1 = src[0];
    Mat element = getStructuringElement(morph.elementType, Size(morph.size1,morph.size2));
    morphologyEx(src1, dst, morph.morphType, element, Point(-1,-1), morph.iteration);
    return 1;
}

pair<funcId,func> functionSet::getRandomFunction() const
{
    map<string, pair<int,func>>::const_iterator it = functions.begin();
    std::advance(it, rand() % functions.size() );

    string name = it->first;
    func function = it->second.second;
    int argumentsNumber = it->second.first;
    funcId id = make_pair(argumentsNumber,name);

    pair<funcId,func> p = make_pair(id,function);

    return p;
}

pair<funcId,func> functionSet::getRandomFunction(int argumentsNumber) const
{
    map<string, pair<int,func>> argFunctions;
    map<string, pair<int,func>>::const_iterator it = functions.begin();
        //Rewrite funciton with the same number of arguments as argumentsNumber
    for(auto it = functions.begin(); it != functions.end();)
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

    string name = it->first;
    func function = it->second.second;
    argumentsNumber = it->second.first;
    funcId id = make_pair(argumentsNumber,name);

    pair<funcId,func> p = make_pair(id,function);

    return p;
}

pair<funcId,morpho> functionSet::getRandomOperation() const
{
    map<string, pair<int,morpho>>::const_iterator it = operations.begin();
    std::advance(it, rand() % operations.size() );

    string name = it->first;
    morpho operation = it->second.second;
    int argumentsNumber = it->second.first;
    funcId id = make_pair(argumentsNumber,name);

    pair<funcId,morpho> p = make_pair(id,operation);

    return p;
}

morphologyParameters functionSet::getRandomMorphParameters() const
{
    morphologyParameters morph;

    morph.size1 = 1 + rand() % 7;
    morph.size2 = 1 + rand() % 7;
    morph.morphType = rand() % 8;
    morph.elementType = rand() % 3;
    morph.iteration = 1 + rand() % 40;

    return morph;
}

void functionSet::clear()
{
    this->functions.clear();
    this->operations.clear();
}

void functionSet::addFunction(string functionName)
{
    map<string, pair<int,func>>::iterator it;
    it = functionsNotAdded.find(functionName);
    pair<string, pair<int,func>> item = make_pair(it->first, it->second);
    this->functions.insert(item);
}

void functionSet::addOperation(string functionName)
{
    map<string, pair<int,morpho>>::iterator it;
    it = operationsNotAdded.find(functionName);
    pair<string, pair<int,morpho>> item = make_pair(it->first, it->second);
    this->operations.insert(item);
}

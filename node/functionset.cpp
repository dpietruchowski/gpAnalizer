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

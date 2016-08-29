#include "population.h"

#include <limits>
#include <algorithm>
#include <functional>
#include <iostream>

using namespace std;

Population::Population(int treeDepth):
    counterTrees_(0), treeDepth_(treeDepth)
{
    counterTrees_ = 0;
}

Population::Population(int size, int treeDepth):
    counterTrees_(0), treeDepth_(treeDepth)
{
    create(size, treeDepth_);
}

Population::~Population()
{
}

void Population::create(int size, int treeDepth)
{
    counterTrees_ = 0;
    for(int i=0; i<size; i++)
    {
        individuals_.push_back( Individual(TreePtr(new Tree(treeDepth,
                                                            counterTrees_++))) );
    }
}

void Population::init(NodeGenerator& generator)
{
    int populationSize = getSize();
    int i = 0;
    for(; i < populationSize/2; ++i)
    {
        individuals_[i].tree->initialize(FULL_INIT, generator);
        emit getAssessedNumber(i);
    }
    for(; i < populationSize; ++i)
    {
        individuals_[i].tree->initialize(GROW_INIT, generator);
        emit getAssessedNumber(i);
    }
}

Tree *Population::getIndividual(int i)
{
    return individuals_[i].tree.get();
}

pair<int,Tree*> Population::getBest()
{
    for(auto &ind: individuals_)
    {
        if(ind.rank == 0)
            return make_pair(ind.score,ind.tree.get());
    }
    return make_pair(0, nullptr);
}

int Population::getScore(int i) const
{
    return individuals_[i].score;
}

int Population::getRank(int i) const
{
    return individuals_[i].rank;
}

void Population::addIndividual(TreePtr newIndividual)
{
    individuals_.push_back( Individual(move(newIndividual)) );
}

void Population::sort()
{
    //score, position
    vector<pair<int,int>> sorted;
    int i = 0;
    for(const auto& ind: individuals_)
    {
        sorted.push_back(make_pair(ind.score, i));
        ++i;
    }
    std::sort(sorted.begin(), sorted.end());

    i = 0;
    for(const auto& ind: sorted)
    {
        individuals_[ind.second].rank = i;
        ++i;
    }

}

void Population::assess(FitnessType type, const cv::Mat &referenceImage,
                        NodeGenerator& generator)
{
    Fitness *fitness = nullptr;
    switch(type)
    {
    case HAUSDORFF_MODIFIED: fitness = HausdorffModified::create(referenceImage);
        break;
    case HAUSDORFF_CANNY: fitness = HausdorffCanny::create(referenceImage);
        break;
    case HAMMING: fitness = Hamming::create(referenceImage);
        break;
    default:
        throw string("Population::assess: Should never get here");
    }
    assess(fitness, generator);

    delete fitness;
}

void Population::assess(FitnessGenerator &generator, NodeGenerator& nodeGenerator)
{

    Fitness *fitness = generator.createRandomPtr();
    assess(fitness, nodeGenerator);

    delete fitness;
}

int Population::getSize()
{
    return individuals_.size();
}

void Population::swap(Population* newPopulation)
{
    individuals_.swap(newPopulation->individuals_);
}

void Population::clear()
{
    individuals_.clear();
    counterTrees_ = 0;
}

void Population::assess(Fitness *fitness, NodeGenerator& generator)
{
    int i = 0;
    for(auto &ind: individuals_)
    {
        cv::Mat result = ind.tree->run();
        threshold(result, result, 125, 255, 0);
        int whitePixels = countNonZero(result);
        int blackPixels = result.total() - whitePixels;

        if( (whitePixels == 0) || (blackPixels == 0) )
        {
            ind.tree = move( TreePtr(new Tree(ind.tree->getDepth(), 0)) );
            ind.tree->initialize(FULL_INIT, generator);
            result = ind.tree->run();
        }

        ind.score = fitness->measure(result);
        emit getAssessedNumber(i);
        emit getBlackPixels(result.total() - cv::countNonZero(result));
        ++i;
    }
    sort();
}

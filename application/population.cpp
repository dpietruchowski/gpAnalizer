#include "population.h"

#include <limits>
#include <algorithm>
#include <functional>
#include <iostream>

using namespace std;

Population::Population()
{
    counterTrees_ = 0;
}

Population::Population(int size, int treeDepth)
{
    create(size, treeDepth);
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
    int populationSize = getSize();int i = 0;
    for(; i < populationSize/2; ++i)
    {
        individuals_[i].tree->initialize(FULL_INIT, generator);
    }
    for(; i < populationSize; ++i)
    {
        individuals_[i].tree->initialize(GROW_INIT, generator);
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

void Population::assess(FitnessType type, const cv::Mat &referenceImage)
{
    Fitness *fitness = nullptr;
    switch(type)
    {
    case HAUSDORFF: fitness = Hausdorff::create(referenceImage);
        break;
    case HAUSDORFF_CANNY: fitness = HausdorffCanny::create(referenceImage);
        break;
    case HAMMING: fitness = Hamming::create(referenceImage);
        break;
    default:
        throw string("Population::assess: Should never get here");
    }
    assess(fitness);

    delete fitness;
}

void Population::assess(FitnessGenerator &generator)
{

    Fitness *fitness = generator.createRandomPtr();
    assess(fitness);

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

void Population::assess(Fitness *fitness)
{
    int i = 0;
    for(auto &ind: individuals_)
    {
        cv::Mat result = ind.tree->run();
        ind.score = fitness->measure(result);
        emit getAssessedNumber(i);
        ++i;
    }
    sort();
}

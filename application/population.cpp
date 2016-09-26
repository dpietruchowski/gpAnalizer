#include "population.h"

#include <limits>
#include <algorithm>
#include <functional>
#include <iostream>

using namespace std;

Population::Population(int treeDepth):
    counterTrees_(0), treeDepth_(treeDepth)
{
    counterTrees_ = 100;
    generator_.registerObject(0.9, HoistMutation::create);
    generator_.registerObject(0.1, CollapseMutation::create);
}

Population::Population(int size, int treeDepth, const cv::Mat& image):
    counterTrees_(0), treeDepth_(treeDepth)
{
    create(size, treeDepth_, image);
    generator_.registerObject(0.9, HoistMutation::create);
    generator_.registerObject(0.1, CollapseMutation::create);
}

Population::~Population()
{
}

void Population::create(int size, int treeDepth, const cv::Mat& image)
{
    counterTrees_ = 0;
    for(int i=0; i<size; i++)
    {
        individuals_.push_back( Individual(TreePtr(new Tree(treeDepth,
                                                            counterTrees_++,
                                                            image))) );
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

int Population::getMutated() const
{
    return mutated_;
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

void Population::mutate(const vector<pair<int,int>> &sorted, Fitness *fitness,
                        NodeGenerator& generator)
{
    emit getOperation("Wymiana");
    int tmp = 0;
    int i = 0;
    for(const auto& ind: sorted)
    {
        if(individuals_[ind.second].tree->getDepth() > 15)
        {
            GeneticOperation* mutation = generator_.createRandomPtr();
            vector<Tree*> parents;
            parents.push_back(individuals_[ind.second].tree.get());
            individuals_[ind.second].tree = move( mutation->reproduce(parents,
                                                              &generator) );
            delete mutation;
        } else
        if(tmp == ind.first)
        {
            mutated_++;
            emit getAssessedNumber(i);

            individuals_[ind.second].tree = move(TreePtr(new Tree(
                                             individuals_[ind.second].tree->getDepth(), 0,
                                             individuals_[ind.second].tree->getImage())));
            individuals_[ind.second].tree->initialize(GROW_INIT, generator);

            cv::Mat result = individuals_[ind.second].tree->run();
            individuals_[ind.second].score = fitness->measure(result);
        } else
            tmp = ind.first;
        ++i;
    }
}

void Population::assess(FitnessType type, const cv::Mat &referenceImage,
                        NodeGenerator& generator, int i)
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
    assess(fitness, generator, i);

    delete fitness;
}

void Population::assess(FitnessGenerator &generator, NodeGenerator& nodeGenerator,
                        int i)
{

    Fitness *fitness = generator.createRandomPtr();
    assess(fitness, nodeGenerator, i);

    delete fitness;
}

int Population::getSize()
{
    return individuals_.size();
}

void Population::swap(Population* newPopulation)
{
    individuals_.swap(newPopulation->individuals_);
    std::swap(katalog_, newPopulation->katalog_);
}

void Population::clear()
{
    individuals_.clear();
    counterTrees_ = 0;
}

void Population::setKatalog(const string &katalog)
{
    katalog_ = katalog;
}

void Population::assess(Fitness *fitness, NodeGenerator& generator, int k)
{
    vector<pair<int,int>> sorted;
    int i = 0;
    for(auto &ind: individuals_)
    {
        cv::Mat result = ind.tree->run();
        threshold(result, result, 125, 255, 0);
        int whitePixels = countNonZero(result);
        int blackPixels = result.total() - whitePixels;

//        if( (whitePixels == 0) || (blackPixels == 0) )
//        {
//            int treeDepth = ind.tree->getDepth();
//            if(treeDepth > 6) treeDepth = 6;
//            ind.tree = move( TreePtr(new Tree(treeDepth, 0,
//                                              ind.tree->getImage())) );
//            ind.tree->initialize(GROW_INIT, generator);
//            result = ind.tree->run();
//        }

        ind.score = fitness->measure(result);
        emit getAssessedNumber(i);
//        blackPixels = result.total() - countNonZero(result);
        sorted.push_back(make_pair(blackPixels, i));
        emit getBlackPixels(blackPixels);
        ++i;
    }
    std::sort(sorted.begin(), sorted.end());
    mutated_ = 0;
//    if(k%50 == 0)
        mutate(sorted, fitness, generator);
    sort();
}

//void Population::savePopulation(int generationNumber, string katalog)
//{
//    string command = "mkdir ";
//    command += katalog + "/population";
//    command += to_string(generationNumber);
//    const char *cstr = command.c_str();
//    system(cstr);
//    int i = 0;
//    for(auto const &p: individuals_)
//    {
//        cv::Mat result = p.tree->run();
//        string folder = katalog + "/population";
//        string nr = to_string(generationNumber);
//        string name = "individual";
//        name = folder + nr + "/" + name + to_string(i) + ".png";
//        cv::imwrite(name,result);
//        i++;
//    }
//}

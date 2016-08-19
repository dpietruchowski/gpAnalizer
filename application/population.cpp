#include "population.h"

using namespace std;

Population::Population()
{
    counterTrees_ = 0;
}

Population::Population(int size, int treeDepth, int generationNumber)
{
    counterTrees_ = 0;
    for(int i=0; i<size; i++)
    {
        individuals_.push_back( TreePtr(new Tree(treeDepth, counterTrees_++)) );
    }
}

Population::~Population()
{
}

void Population::erase(int i)
{
    vector<TreePtr>::iterator it = individuals_.begin();
    for (int n = 0; n < i; n++)
        ++it;
    individuals_.erase(it);
}

void Population::create(int size, int treeDepth, int generationNumber)
{
    counterTrees_ = 0;
    for(int i=0; i<size; i++)
    {
        individuals_.push_back( TreePtr(new Tree(treeDepth, counterTrees_++)) );
    }
}

void Population::init(NodeGenerator& generator)
{
    int populationSize = getSize();
    for (initializeIt_ = individuals_.begin();
         initializeIt_ < individuals_.begin()+populationSize/2;
         ++initializeIt_)
    {
       (*initializeIt_)->initialize(FULL_INIT, generator);
    }

    for (; initializeIt_ < individuals_.end(); ++initializeIt_)
    {
       (*initializeIt_)->initialize(GROW_INIT, generator);
    }
}

Tree* Population::getIndividual(int i)
{
    return individuals_[i].get();
}

void Population::addIndividual(TreePtr newIndividual, int generationNumber)
{
    individuals_.push_back( move(newIndividual) );
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

void Population::savePopulation(int generationNumber, string katalog)
{
    string command = "mkdir ";
    command += katalog + "/population";
    command += to_string(generationNumber);
    const char *cstr = command.c_str();
    system(cstr);
    int i = 0;
    for(auto const &p: individuals_)
    {
        cv::Mat result = p->run();
        string folder = "images/population";
        string nr = to_string(generationNumber);
        string name = "individual";
        name = folder + nr + "/" + name + to_string(i) + ".png";
        cv::imwrite(name,result);
        i++;
    }
}

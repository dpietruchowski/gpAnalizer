#ifndef POPULATION_H
#define POPULATION_H

#include <vector>
#include "tree.h"

class Population
{
public:
    Population();
    Population(int size, int treeDepth, int generationNumber);
    ~Population();

public:
    void erase(int i);
    void create(int size, int treeDepth, int generationNumber);
    void init(NodeGenerator& generator);
    void addIndividual(TreePtr newIndividual, int generationNumber);
    Tree* getIndividual (int i);
    int getSize();
    void swap(Population* newPopulation);
    void clear();
    void savePopulation(int generationNumber, std::string katalog);

private:
    std::vector<TreePtr> individuals_;
    std::vector<TreePtr>::iterator initializeIt_;
private:
    int counterTrees_;
};

#endif // POPULATION_H

#ifndef POPULATION_H
#define POPULATION_H

#include <vector>
#include "tree.h"
#include "functions.h"

class population
{
private:
    std::vector <tree_ptr> individuals;
    std::vector <tree_ptr>::iterator initializeIt;
private:
    int counterTrees;
public:
    population();
    population(int size, int treeDepth, int generationNumber);
    ~population();

public:
    void erase(int i);
    void create(int size, int treeDepth, int generationNumber);
    void init(const functionSet& functions, const terminalSet& terminals);
    void addIndividual(tree_ptr newIndividual, int generationNumber);
    tree* getIndividual (int i);
    int getSize();
    void swap(population* newPopulation);
    void clear();
    void show();
    void savePopulation(int generationNumber, std::string katalog);

};

#endif // POPULATION_H

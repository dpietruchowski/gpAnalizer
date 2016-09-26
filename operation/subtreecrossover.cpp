#include "subtreecrossover.h"

#include <iostream>
using namespace std;

GeneticOperation *SubtreeCrossover::create()
{
    return new SubtreeCrossover();
}

SubtreeCrossover::SubtreeCrossover()
{

}

TreePtr SubtreeCrossover::crossover(Tree *parent1, Tree *parent2) const
{    
    if(parent1->getDepth() < parent2->getDepth())
        return directCrossover(parent1, parent2);
    else return directCrossover(parent2, parent1);
}

TreePtr SubtreeCrossover::directCrossover(Tree *parent1, Tree *parent2) const
{
    int mutationPoint1 = getRandomMutationPoint(parent1);

    TreePtr newSubtree = move(parent1->cloneSubtree(mutationPoint1, 0));
    int subtreeDepth = newSubtree->getDepth();

    std::vector<int> mutationPoints;
    for(int i = 1; i < parent2->getSize(); ++i)
    {
        int depth = parent2->getSubtreeDepth(i);
        if (depth == subtreeDepth)
            mutationPoints.push_back(i);
    }
    int mutationPoint2;
    if(mutationPoints.empty() == false)
    {
        int random = rand() % mutationPoints.size();
        mutationPoint2 = mutationPoints[random];
    } else
    {
        mutationPoint2 = getRandomMutationPoint(parent2);
    }

    TreePtr offspring = move( parent2->clone(0) );
    offspring->setSubtree(mutationPoint2, *newSubtree->getRoot() );

    return move(offspring);
}

#include "subtreecrossover.h"

GeneticOperation *SubtreeCrossover::create()
{
    return new SubtreeCrossover();
}

SubtreeCrossover::SubtreeCrossover()
{

}

TreePtr SubtreeCrossover::crossover(Tree *parent1, Tree *parent2) const
{
    int mutationPoint = 1;
    if (parent1->getSize() > parent2->getSize())
    {
        mutationPoint = getRandomMutationPoint(parent2);
    } else
    {
        mutationPoint = getRandomMutationPoint(parent1);
    }

    TreePtr newSubtree = move(parent1->cloneSubtree(mutationPoint, 0));
    TreePtr offspring = move( parent2->clone(0) );
    offspring->setSubtree(mutationPoint, *newSubtree->getRoot() );

    return move(offspring);
}


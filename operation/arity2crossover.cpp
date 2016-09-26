#include "arity2crossover.h"

GeneticOperation *Arity2Crossover::create()
{
    return new Arity2Crossover();
}

Arity2Crossover::Arity2Crossover()
{

}

TreePtr Arity2Crossover::crossover(Tree *, Tree *) const
{
    //    int mutationPoint = getRandomMutationPoint(parent1);
    //    TreePtr newSubtree1 = move(parent1->copySubtree(mutationPoint));

    //    mutationPoint = getRandomMutationPoint(parent2);
    //    TreePtr newSubtree2 = move(parent2->copySubtree(mutationPoint));

    //    NodePtr newRoot( new FunctionNode(functions.getRandomFunction(2)) );
    //    newRoot->addChild(newSubtree1->returnRoot());
    //    newRoot->addChild(newSubtree2->returnRoot());

    //    TreePtr offspring( new tree(3) );
    //    offspring->setRoot( move(newRoot) );

    //    return move(offspring);
    return nullptr;
}


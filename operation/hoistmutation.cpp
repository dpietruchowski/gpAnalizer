#include "hoistmutation.h"
#include <iostream>

using namespace std;

GeneticOperation *HoistMutation::create()
{
    return new HoistMutation();
}

HoistMutation::HoistMutation()
{

}

TreePtr HoistMutation::mutate(Tree *parent)
{
    int mutationPoint = getRandomMutationPoint(parent);
    TreePtr offspring = parent->clone(0);
    int parentDepth = parent->getDepth();
    if(parentDepth > 3)
    {
        int subtreeSize = parent->getSubtreeDepth(mutationPoint);
        while(subtreeSize < 2)
        {
            mutationPoint = getRandomMutationPoint(parent);
            subtreeSize = parent->getSubtreeDepth(mutationPoint);
        }
        //Offspring is subtree with randomly picked node
        offspring = move(parent->cloneSubtree(mutationPoint, 0));
    }

    return move(offspring);
}


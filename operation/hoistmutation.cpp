#include "hoistmutation.h"

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
        //Offspring is subtree with randomly picked node
    TreePtr offspring = parent->cloneSubtree(mutationPoint, 0);
    if(offspring->getRoot()->getId().type == TERMINAL_NODE)
        offspring = move(parent->clone(0));

    return move(offspring);
}


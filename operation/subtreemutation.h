#ifndef SUBTREEMUTATION_H
#define SUBTREEMUTATION_H

#include "mutation.h"

class SubtreeMutation : public Mutation
{
public:
    static GeneticOperation* create();
    SubtreeMutation();
private:
    TreePtr mutate(Tree* parent);
};

#endif // SUBTREEMUTATION_H

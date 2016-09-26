#ifndef COLLAPSEMUTATION_H
#define COLLAPSEMUTATION_H

#include "mutation.h"

class CollapseMutation : public Mutation
{
public:
    static GeneticOperation* create();
    CollapseMutation();
private:
    TreePtr mutate(Tree* parent);
};

#endif // COLLAPSEMUTATION_H

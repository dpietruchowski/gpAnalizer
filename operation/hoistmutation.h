#ifndef HOISTMUTATION_H
#define HOISTMUTATION_H

#include "mutation.h"

class HoistMutation : public Mutation
{
public:
    static GeneticOperation* create();
    HoistMutation();
private:
    TreePtr mutate(Tree* parent);
};

#endif // HOISTMUTATION_H

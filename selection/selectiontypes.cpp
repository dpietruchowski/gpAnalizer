#include "selectiontypes.h"

Score::Score(int rank, int value, double normalized):
rank(rank), value(value), normalized(normalized)
{
}

Score FitnessScore::add(int rank, int fitness) const
{
    return Score{rank, fitness, 0};
}

Score RankScore::add(int rank, int) const
{
    return Score{rank, rank, 0};
}

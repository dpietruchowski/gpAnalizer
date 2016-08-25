#ifndef ROULETTESELECTION_H
#define ROULETTESELECTION_H

#include "selection.h"

class RouletteSelection: public Selection
{
public:
    RouletteSelection(ScoreType*);

private:
    int select(const Scores&) const;
    void calcScores(Scores&, int worstScore);
};

class RankRouletteSelection: public RouletteSelection
{
public:
    RankRouletteSelection(): RouletteSelection(new RankScore()) {}
};

class FitnessRouletteSelection: public RouletteSelection
{
public:
    FitnessRouletteSelection(): RouletteSelection(new FitnessScore()) {}
};

#endif

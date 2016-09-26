#include "rouletteselection.h"
#include "selectiontypes.h"

static SelectionRandomGenerator generator;

#include <cstdlib>

RouletteSelection::RouletteSelection(ScoreType* scoreType):
    Selection(scoreType)
{
}


//TODO use library to choose by probabilities
int RouletteSelection::select(const Scores& scores) const
{
    double decision = const_cast<SelectionRandomGenerator&> (generator)();
    double sum = 0;
    int i = 0;
    double sumofScores = 0;
    for(const auto& score: scores)
    {
        sumofScores += score.normalized;
    }
    for(const auto& score: scores)
    {
        sum += score.normalized;
        if(decision <= sum)
            return i;
        i++;
    }
    //should never get here
    return i;
}

void RouletteSelection::calcScores(Scores& scores, int worstScore)
{
    int sumOfScores = 0;
    for(auto& score : scores)
    {
        score.value = (worstScore - score.value + 1);
        sumOfScores += score.value;
    }

    for(auto& score : scores)
        score.normalized = static_cast<double>(score.value * 100) / sumOfScores;
}

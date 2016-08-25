#include "selection.h"

Selection::Selection(ScoreType* scoreType)
{
    scoreType_ = scoreType;
    worstScore_ = 0;
}

Selection::~Selection()
{
    delete scoreType_;
}

void Selection::add(int rank, int fitness)
{
    Score Score = scoreType_->add(rank, fitness);
    if(Score.value > worstScore_)
        worstScore_ = Score.value;

    scores_.push_back(Score);
}

void Selection::calcScores()
{
    calcScores(scores_, worstScore_);
}

int Selection::select() const
{
    return select(scores_);
}

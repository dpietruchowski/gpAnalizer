#include "selection.h"

Selection::Selection(ScoreType* scoreType)
//    generator_()
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
    Score score = scoreType_->add(rank, fitness);
    if(score.value > worstScore_)
        worstScore_ = score.value;

    scores_.push_back(score);
}

void Selection::calcScores()
{
    calcScores(scores_, worstScore_);
}

int Selection::select() const
{
    return select(scores_);
}

#ifndef MEPSELECTION_H
#define MEPSELECTION_H

#include "selectiontypes.h"

class Selection
{
public:
    Selection(ScoreType*);
    virtual ~Selection();

    void add(int rank, int score);
    void calcScores();
    Scores getScores() { return scores_; }
    int select() const;
private:
    virtual int select(const Scores&) const = 0;
    virtual void calcScores(Scores&, int worstScore) = 0;
private:
    Scores scores_;
    ScoreType* scoreType_;
    int worstScore_;
};

#endif

#ifndef SELECTIONTYPES_H
#define SELECTIONTYPES_H

#include <vector>
#include <string>
#include "node/nodeid.h"

enum SelectionType
{
    FITNESS_ROULETTESELECTION,
    //RANK_ROULETTESELECTION,
    TOURNAMENTSELECTION
};

struct Score
{
    int rank;
    int value;
    double normalized;
    Score(int, int, double);
    std::string toString() const
    {
        std::string sScore = std::to_string(rank);
        sScore += " ";
        sScore += to_string_with_precision(value, 3);
        sScore += " ";
        sScore += to_string_with_precision(normalized, 3);

        return sScore;
    }
};
typedef std::vector<Score> Scores;

struct ScoreType
{
    virtual ~ScoreType() {}
    virtual Score add(int rank, int fitness) const = 0;
};

struct FitnessScore: public ScoreType
{
    Score add(int rank, int fitness) const;
};

struct RankScore: public ScoreType
{
    Score add(int rank, int fitness) const;
};

#endif

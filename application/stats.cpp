#include "stats.h"
#include "fstream"

Stats::Stats(int size):
    size_(size), rankSelected_(size, 0)
{
}

void Stats::save(const std::string &filePath)
{
    std::ofstream plik(filePath);

    plik << getFitnessStats() << std::endl;
    plik << getNNodesStats() << std::endl;
    plik << getDepthStats() << std::endl;
    plik << getRankSelectedStats() << std::endl;
    plik << getBlackStats() << std::endl;
    plik << getRank() << std::endl;
    plik << getMutated() << std::endl;

    plik.close();
}

std::string Stats::getFitnessStats() const
{
    std::string fitness = "fitness=";
    fitness += vectorToString(scores_);

    return fitness;
}

std::string Stats::getNNodesStats() const
{
    std::string nNodes = "nnodes=";
    nNodes += vectorToString(nNodes_);

    return nNodes;
}

std::string Stats::getDepthStats() const
{
    std::string depth = "depth=";
    depth += vectorToString(depth_);

    return depth;
}

std::string Stats::getRankSelectedStats() const
{
    std::string rankSelected = "sranks=";
    rankSelected += vectorToString(rankSelected_);

    return rankSelected;
}

std::string Stats::getBlackStats() const
{
    std::string black = "black=";
    black += vectorToString(black_);

    return black;
}

std::string Stats::getRank() const
{
    std::string rank = "rank=";
    rank += vectorToString(rank_);

    return rank;
}

std::string Stats::getMutated() const
{
    std::string mutated = "mutated=";
    mutated += vectorToString(mutated_);

    return mutated;
}

void Stats::addFitness(int score)
{
    scores_.push_back(score);
}

void Stats::addNNodes(int nNodes)
{
    nNodes_.push_back(nNodes);
}

void Stats::addDepth(int depth)
{
    depth_.push_back(depth);
}

void Stats::addBlack(int black)
{
    black_.push_back(black);
}

void Stats::addRank(int rank)
{
    rank_.push_back(rank);
}

void Stats::addMutated(int mutated)
{
    mutated_.push_back(mutated);
}

void Stats::incRankSelected(int rank)
{
    rankSelected_[rank]++;
}

void Stats::clear()
{
    scores_.clear();
    nNodes_.clear();
    depth_.clear();
    black_.clear();
    rank_.clear();
    mutated_.clear();
    rankSelected_ = std::vector<int>(size_, 0);
}

std::string Stats::vectorToString(const std::vector<int> &vec) const
{
    std::string sVec = "[";
    sVec += std::to_string(vec[0]);
    for(int i = 1; i < static_cast<int>(vec.size()); ++i)
    {
        sVec += ",";
        sVec += std::to_string(vec[i]);
    }
    sVec += "]";
    sVec += ";";

    return sVec;
}

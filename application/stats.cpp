#include "stats.h"
#include "fstream"

using namespace std;

Stats::Stats(int size):
    size_(size), rankSelected_(size, 0)
{
}

void Stats::save(const string &filePath)
{
    ofstream plik(filePath);

    plik << getFitnessStats() << endl;
    plik << getNNodesStats() << endl;
    plik << getDepthStats() << endl;
    plik << getRankSelectedStats() << endl;
    plik << getNonZerosStats() << endl;

    plik.close();
}

string Stats::getFitnessStats() const
{
    string fitness = "fitness=";
    fitness += vectorToString(scores_);

    return fitness;
}

string Stats::getNNodesStats() const
{
    string nNodes = "nnodes=";
    nNodes += vectorToString(nNodes_);

    return nNodes;
}

string Stats::getDepthStats() const
{
    string depth = "depth=";
    depth += vectorToString(depth_);

    return depth;
}

string Stats::getRankSelectedStats() const
{
    string rankSelected = "sranks=";
    rankSelected += vectorToString(rankSelected_);

    return rankSelected;
}

string Stats::getNonZerosStats() const
{
    string nonZeros = "nonzeros=";
    nonZeros += vectorToString(nonZeros_);

    return nonZeros;
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

void Stats::addNonZeros(int nonZeros)
{
    nonZeros_.push_back(nonZeros);
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
    nonZeros_.clear();
    rankSelected_ = vector<int>(size_, 0);
}

string Stats::vectorToString(const std::vector<int> &vec) const
{
    string sVec = "[";
    sVec += to_string(vec[0]);
    for(int i = 1; i < static_cast<int>(vec.size()); ++i)
    {
        sVec += ",";
        sVec += to_string(vec[i]);
    }
    sVec += "]";
    sVec += ";";

    return sVec;
}

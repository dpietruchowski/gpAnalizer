#ifndef STATS_H
#define STATS_H

#include <vector>
#include <string>

class Stats
{
public:
    Stats(int size);

    void save(const std::string& filePath);

    void addFitness(int score);
    void addNNodes(int nNodes);
    void addDepth(int depht);
    void addBlack(int pixel);
    void incRankSelected(int rank);
    void clear();

private:
    std::string vectorToString(const std::vector<int>& vec) const;

    std::string getFitnessStats() const;
    std::string getNNodesStats() const;
    std::string getDepthStats() const;
    std::string getRankSelectedStats() const;
    std::string getBlackStats() const;

private:
    int size_;
    std::vector<int> scores_;
    std::vector<int> nNodes_;
    std::vector<int> depth_;
    std::vector<int> rankSelected_;
    std::vector<int> black_;

};

#endif // STATS_H

#ifndef POPULATION_H
#define POPULATION_H

#include <vector>
#include "tree.h"
#include "fitness/fitness_h.h"
#include "selection/selection_h.h"
#include <limits>
#include <QtCore>

struct Individual
{
    int rank;
    int score;
    TreePtr tree;
    Individual(TreePtr treee):
        rank(-1), score(std::numeric_limits<int>::max())
    { tree = move(treee); }
};

class Population : public QObject
{
    Q_OBJECT
public:
    Population();
    Population(int size, int treeDepth);
    ~Population();

public:
    void create(int size, int treeDepth);
    void init(NodeGenerator& generator);
    Tree* getIndividual(int i);
    std::pair<int,Tree*> getBest();
    int getScore(int i) const;
    int getRank(int i) const;
    void assess(FitnessType type, const cv::Mat& referenceImage);
    void assess(FitnessGenerator& generator);

    void addIndividual(TreePtr newIndividual);
    int getSize();
    void swap(Population* newPopulation);
    void clear();

private:
    void assess(Fitness* fitness);
    void sort();

private:
    std::vector<Individual> individuals_;
    std::vector<Individual>::iterator initializeIt_;
private:
    int counterTrees_;

signals:
    void getAssessedNumber(int);
};

#endif // POPULATION_H

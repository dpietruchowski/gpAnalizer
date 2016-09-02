#ifndef POPULATION_H
#define POPULATION_H

#include <vector>
#include "tree.h"
#include "fitness/fitness_h.h"
#include "selection/selection_h.h"
#include "operation/geneticoperation_h.h"
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
    Population(int treeDepth);
    Population(int size, int treeDepth, const cv::Mat& image);
    ~Population();

public:
    void create(int size, int treeDepth, const cv::Mat &image);
    void init(NodeGenerator& generator);
    Tree* getIndividual(int i);
    std::pair<int,Tree*> getBest();
    int getScore(int i) const;
    int getRank(int i) const;
    int getMutated() const;
    void assess(FitnessType type, const cv::Mat& referenceImage,
                NodeGenerator& generator, int i);
    void assess(FitnessGenerator& generator, NodeGenerator& nodeGenerator, int i);

    void addIndividual(TreePtr newIndividual);
    int getSize();
    void swap(Population* newPopulation);
    void clear();
    void setKatalog(const std::string& katalog);

    void savePopulation(int generationNumber, std::string katalog);
private:
    void assess(Fitness* fitness, NodeGenerator& generator, int i);
    void sort();
    void mutate(const std::vector<std::pair<int,int>>& sorted,
                Fitness *fitness,
                NodeGenerator& generator);


private:
    std::vector<Individual> individuals_;
    std::vector<Individual>::iterator initializeIt_;
private:
    GeneticOperationGenerator generator_;
    int counterTrees_;
    int treeDepth_;
    std::string katalog_;
    int mutated_;

signals:
    void getAssessedNumber(int);
    void getOperation(std::string);
    void getBlackPixels(int);
};

#endif // POPULATION_H

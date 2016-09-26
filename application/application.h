#ifndef APPLICATION_H
#define APPLICATION_H

#include "population.h"
#include "stats.h"
#include "selection/selection.h"
#include "operation/geneticoperation_h.h"
#include <vector>
#include <QtCore>
#include <cmath>

typedef std::pair<double,Tree*> double_tree;

struct BestIndividual
{
    int generationNumber;
    int individualNumber;
    int programResult;
    cv::Mat image;
    BestIndividual():
        generationNumber(0), individualNumber(0), programResult(100000000)
    {}

};
Q_DECLARE_METATYPE(BestIndividual)

struct GeneticParameters
{
    int populationSize;
    int treeDepth;
    int tournamentSize;
    SelectionType selectType;
    FitnessType fitType;
    GeneticParameters(int populationSize, int treeDepth, int tournamentSize,
                      SelectionType selectionType, FitnessType fitnessType):
        populationSize(populationSize), treeDepth(treeDepth),
        tournamentSize(tournamentSize), selectType(selectionType),
        fitType(fitnessType) {}
    GeneticParameters():
        populationSize(0), treeDepth(0), tournamentSize(0),
        selectType(FITNESS_ROULETTESELECTION), fitType(HAMMING) {}
    GeneticParameters(const GeneticParameters&) = default;
};

struct GeneticNodeProbabilities
{
    double function;
    double morpho;
    double thresh;
    GeneticNodeProbabilities(const GeneticNodeProbabilities&) = default;
    GeneticNodeProbabilities():
        function(0), morpho(0), thresh(0) {}
    GeneticNodeProbabilities(double function, double morpho, double thresh):
        function(function), morpho(morpho), thresh(thresh) {}
    double sum() const
    { double s = function + morpho + thresh;
      return std::round(s * 1000) / 1000; }
};

struct GeneticOperationProbabilities
{
    double subtreeMutation;
    double nodeMutation;
    double hoistMutation;
    double collapseMutation;
    double subtreeCrossover;
    double copy;

    GeneticOperationProbabilities(const GeneticOperationProbabilities&) = default;
    GeneticOperationProbabilities():
        subtreeMutation(0), nodeMutation(0), hoistMutation(0),
        collapseMutation(0), subtreeCrossover(0),
        copy(0) {}
    GeneticOperationProbabilities(double subtree, double node, double hoist,
                                  double collapse, double subtreeCrossover,
                                  double copy):
        subtreeMutation(subtree), nodeMutation(node), hoistMutation(hoist),
        collapseMutation(collapse), subtreeCrossover(subtreeCrossover),
        copy(copy) {}
    double sum() const
    { double s = subtreeMutation + nodeMutation + hoistMutation + collapseMutation +
                subtreeCrossover + copy;
      return std::round(s * 1000) / 1000; }
};

struct StopCriteriumParameters
{
    int nGenerations;
    int minResult;
    StopCriteriumParameters(const StopCriteriumParameters&) = default;
    StopCriteriumParameters(int nGenerations, int minResult):
        nGenerations(nGenerations), minResult(minResult) {}
    StopCriteriumParameters():
        nGenerations(0), minResult(0) {}
};



class Application: public QThread
{
    Q_OBJECT

private:
    std::string katalog_;
    bool isStopped_;
    int generationNumber_;

    StopCriteriumParameters stopParam_;

    cv::Mat referenceImage_;
    cv::Mat inputImage_;
    GeneticParameters geneticParam_;
    Selection *selection_;
    NodeGenerator generator_;
    GeneticOperationGenerator operationGenerator_;

    Population actualPopulation_;
    Population newPopulation_;

    /*
     * Best program result in GP
     */
    Stats stats_;
    BestIndividual best_;
    TreePtr bestProgram_;
public:
    cv::Mat bestIndividualOutput;
public:
    Application();
    Application(const Application& rhs);
    Application& operator =(const Application& rhs);
    void swap(Application& rhs);
    ~Application();
public:
    void init();
    void evolution();
    TreePtr createNewIndividual();
    void saveBest(std::string &program);
    void load(std::string &name);
    void checkIfBetterSolution();
public:
    void assessIndividuals();
    Tree* selectIndividual();

public:
    void run();
    void stop();
    void setGeneticOperationProbabilities(
            const GeneticOperationProbabilities& probabilities);
    void setGeneticParameters(const GeneticParameters& param);
    void setStopCriterium(const StopCriteriumParameters& param);
    void setNodeProbabilities(const GeneticNodeProbabilities& probabilities);
    void setInputImage(const cv::Mat& inputImage);
    void setReferenceImage(const cv::Mat& referenceImage);
    void setKatalog(std::string katalog);
    void getKatalog(std::string& katalog);
//    void clearKatalog();

private:
    int getRandomMutationPoint(Tree* parent);
private slots:
    void getAssessedNumber(int number);
    void getBlackPixels(int pixels);
    void setOperation(std::string);

signals:
    void throwException(std::string);
    void getAssessed(int);
    void getGeneration(int);
    void getIndividual(int);
    void getOperation(std::string);
    void newBestProgram(BestIndividual);

};

#endif // APPLICATION_H

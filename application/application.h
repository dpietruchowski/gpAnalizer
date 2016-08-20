#ifndef APPLICATION_H
#define APPLICATION_H

#include "population.h"
#include <map>
#include <vector>
#include <fstream>
#include <QtCore>

typedef std::pair<double,Tree*> double_tree;

struct bestIndividual
{
    int generationNumber;
    int individualNumber;
    int programResult;
    cv::Mat image;
};

enum SelectionType
{
    SELECTION,
    RANK_SELECTION,
    TOURNAMENT_SELECTION,
    ROULETTE_SELECTION
};

enum MutationType
{
    SUBTREE_MUTATION,
    NODE_MUTATION,
    HOIST_MUTATION,
    COLLAPSE_MUTATION
};

enum CrossoverType
{
    SUBTREE_CROSSOVER,
    ARITY2_CROSSOVER
};

enum FitnessType
{
    HAUSDORFF_SMALL,
    HAUSDORFF_CANNY,
    HAMMING
};

class Application: public QThread
{
    Q_OBJECT

private:
    std::string katalog;
    bool isStopped;

    Population actualPopulation;
    Population newPopulation;
    int generationNumber;
    /*
     * Program parameters
     */
    int maxGenerationNumber;
    int minResult;
    /*
     * Genetic parameters
     */
    cv::Mat referenceImage;
    cv::Mat inputImage;
    int populationSize;
    int treeDepth;
    double hoistMutationProbability;
    double nodeMutationProbability;
    double collapseMutationProbability;
    double subtreeMutationProbability;
    double subtreeCrossoverProbability;
    double arity2CrossoverProbability;
    int tournamentSize;
    int parentsSize;
    SelectionType selectType;
    FitnessType fitType;
    NodeGenerator generator;
    /*
     * Best program result in GP
     */
    bestIndividual best;
    TreePtr bestProgram;
public:
        //the less, the better fit individual
    std::map<int,Tree*> sortedIndividuals;
        //the less, the better fit individual
    std::map<int,Tree*> parents;
        //the more, the better fit individual
    std::vector<double_tree> normalizedIndividuals;
        //the less, the better fit individual
    std::vector<double_tree> standardizedIndividuals;
    cv::Mat bestIndividualOutput;
public:
    Application();
public:
    int fitnessMeasure(Tree* individual);
public:
    void init();
    void evolution();
    TreePtr createNewIndividual();
    void saveBest(std::string &program);
    void load(std::string &name);
    void checkIfBetterSolution();
public:
    /*
     * Fitness functions
     */
    int distance(const std::vector<cv::Point> & a,
                 const std::vector<cv::Point> & b);
    int distanceHausdorff(const std::vector<cv::Point> & a,
                          const std::vector<cv::Point> & b);
    int fitnessHausdorffCanny(const cv::Mat& A, const cv::Mat& B);
    int fitnessHausdorffSmall(const cv::Mat& A, const cv::Mat& B);
    int fitnessHamming(const cv::Mat& A, const cv::Mat& B);
    /*!
     * \brief assessIndividuals Terminate all individuals
     * and sort them by fitness function.
     * Individuals are sorted in sortedIndividuals map.
     */
    void assessIndividuals();
    /*!
     * \brief normalizeFitness for all individuals normalize fit.
     * Sum of all fit is equal 1.
     * The higher fit, the better individual.
     * Individuals are normalized in normalizedIndividuals vector.
     */
    void normalizeFitness();
    /*!
     * \brief standardizeFitness for all individuals standardize fit.
     * Number between 0 - 1;
     * The lower fit, the better individual.
     * Individuals are standardized in standardizedIndividuals vector.
     */
    void standardizeFitness();
    void pickParents();
    /*
     * Mutation functions
     */
    /*!
     * \brief subtreeMutation The offspring is copy of parent
     * with randomly generated subtree instead of subtree with root,
     * which is randomly picked node.
     * \param parent Offspring parent.
     * \return Offspring.
     */
    TreePtr subtreeMutation(Tree* parent);
    /*!
     * \brief hoistMutation Offspring is copy of
     * randomly picked subtree of parent.
     * \param parent Offspring parent.
     * \return Offspring.
     */
    TreePtr hoistMutation(Tree* parent);
    /*!
     * \brief nodeMutation Offspring is copy of parent with
     * randomly changed one node.
     * \param parent Offspring parent.
     * \return Offspring.
     */
    TreePtr nodeMutation(Tree* parent);
    /*!
     * \brief collapseMutation Offpring is copy of parent.
     * One of the node is changed to terminal node.
     * \param parent Offspring parent.
     * \return Offspring.
     */
    TreePtr collapseMutation(Tree* parent);
    /*
     * Crossover functions
     */
    TreePtr subtreeCrossover(Tree* parent1, Tree* parent2);
    TreePtr arity2Crossover(Tree* parent1, Tree* parent2);
    /*
     * Selection functions
     */
    Tree* selectIndividual();
    Tree* selectIndividualByFitenss();
    Tree* selectIndividualByTournament(int tournamentSize);
    Tree* selectIndividualFromParents();

public:
    void run();
    void stop();
    void setMutationParameters(double hoistMutationProbability,
                               double nodeMutationProbability,
                               double collapseMutationProbability,
                               double subtreeMutationProbability);
    void setCrossoverParameters(double subtreeCrossoverProbability,
                                double arity2CrossoverProbability);
    void setParameters(int populationSize, SelectionType selectType, FitnessType fitType);
    void setFunctionSet(std::vector<std::string> functionSet);
    void setImages(std::string inputImage, std::string referenceImage);
    void setKatalog(std::string katalog);

private:
    int getRandomMutationPoint(Tree* parent);

signals:
    void getGeneration(int);
    void getIndividual(int);
    void getOperation(std::string);
    void newBestProgram(bestIndividual);

};

#endif // APPLICATION_H

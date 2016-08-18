#ifndef APPLICATION_H
#define APPLICATION_H

#include "population.h"
#include "functions.h"
#include <map>
#include <vector>
#include <fstream>
#include <QtCore>

typedef std::pair<double,tree*> double_tree;

struct bestIndividual
{
    int generationNumber;
    int individualNumber;
    int programResult;
    Mat image;
};

enum selectionType
{
    SELECTION,
    RANK_SELECTION,
    TOURNAMENT_SELECTION,
    ROULETTE_SELECTION
};

enum mutationType
{
    SUBTREE_MUTATION,
    NODE_MUTATION,
    HOIST_MUTATION,
    COLLAPSE_MUTATION
};

enum crossoverType
{
    SUBTREE_CROSSOVER,
    ARITY2_CROSSOVER
};

enum fitnessType
{
    HAUSDORFF_SMALL,
    HAUSDORFF_CANNY,
    HAMMING
};

class application: public QThread
{
    Q_OBJECT

private:
    std::string katalog;
    bool isStopped;

    population actualPopulation;
    population newPopulation;
    functionSet functions;
    terminalSet terminals;
    int generationNumber;
    /*
     * Program parameters
     */
    int maxGenerationNumber;
    int minResult;
    /*
     * Genetic parameters
     */
    Mat referenceImage;
    Mat inputImage;
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
    selectionType selectType;
    fitnessType fitType;
    /*
     * Best program result in GP
     */
    bestIndividual best;
    tree_ptr bestProgram;
public:
        //the less, the better fit individual
    std::map<int,tree*> sortedIndividuals;
        //the less, the better fit individual
    std::map<int,tree*> parents;
        //the more, the better fit individual
    std::vector<double_tree> normalizedIndividuals;
        //the less, the better fit individual
    std::vector<double_tree> standardizedIndividuals;
    Mat bestIndividualOutput;
public:
    application();
public:
    int fitnessMeasure(tree* individual);
public:
    void init();
    void evolution();
    tree_ptr createNewIndividual();
    void saveBest(std::string &program);
    void checkIfBetterSolution();
public:
    /*
     * Fitness functions
     */
    int distance(const std::vector<Point> & a, const std::vector<Point> & b);
    int distanceHausdorff(const std::vector<Point> & a, const std::vector<Point> & b);
    int fitnessHausdorffCanny(const Mat& A, const Mat& B);
    int fitnessHausdorffSmall(const Mat& A, const Mat& B);
    int fitnessHamming(const Mat& A, const Mat& B);
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
    tree_ptr subtreeMutation(tree* parent);
    /*!
     * \brief hoistMutation Offspring is copy of
     * randomly picked subtree of parent.
     * \param parent Offspring parent.
     * \return Offspring.
     */
    tree_ptr hoistMutation(tree* parent);
    /*!
     * \brief nodeMutation Offspring is copy of parent with
     * randomly changed one node.
     * \param parent Offspring parent.
     * \return Offspring.
     */
    tree_ptr nodeMutation(tree* parent);
    /*!
     * \brief collapseMutation Offpring is copy of parent.
     * One of the node is changed to terminal node.
     * \param parent Offspring parent.
     * \return Offspring.
     */
    tree_ptr collapseMutation(tree* parent);
    /*
     * Crossover functions
     */
    tree_ptr subtreeCrossover(tree* parent1, tree* parent2);
    tree_ptr arity2Crossover(tree* parent1, tree* parent2);
    /*
     * Selection functions
     */
    tree* selectIndividual();
    tree* selectIndividualByFitenss();
    tree* selectIndividualByTournament(int tournamentSize);
    tree* selectIndividualFromParents();
public:
    void run();
    void stop();
    void setMutationParameters(double hoistMutationProbability,
                               double nodeMutationProbability,
                               double collapseMutationProbability,
                               double subtreeMutationProbability);
    void setCrossoverParameters(double subtreeCrossoverProbability,
                                double arity2CrossoverProbability);
    void setParameters(int populationSize, selectionType selectType, fitnessType fitType);
    void setFunctionSet(std::vector<std::string> functionSet);
    void setImages(std::string inputImage, std::string referenceImage);
    void setKatalog(std::string katalog);

private:
    int getRandomMutationPoint(tree* parent);

signals:
    void getGeneration(int);
    void getIndividual(int);
    void getOperation(std::string);
    void newBestProgram(bestIndividual);

};

#endif // APPLICATION_H

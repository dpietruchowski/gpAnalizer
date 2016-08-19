#include "application.h"

#include <stdlib.h>
#include <QtCore>
#include <iostream>

using namespace std;
using namespace cv;

Application::Application()
{
    string katalog = "rejestracja";

    string command = "rm -r  ";
    command += katalog;
    const char *cstr = command.c_str();
    system(cstr);
    command = "mkdir ";
    command += katalog;
    cstr = command.c_str();
    system(cstr);

    this->setKatalog(katalog);
    this->isStopped = 0;

    this->maxGenerationNumber = 300;
    this->minResult = 2;
    this->best.programResult = 10000000;

    this->generationNumber = 0;
    this->populationSize = 200;
    this->treeDepth = 8;
    this->hoistMutationProbability = 0.01;
    this->nodeMutationProbability = 0.07;
    this->collapseMutationProbability = 0.01;
    this->subtreeMutationProbability = 0.1;
    this->subtreeCrossoverProbability = 0.7;
    this->arity2CrossoverProbability = 0.01;
    this->selectType = ROULETTE_SELECTION;
    this->fitType = HAMMING;
    this->tournamentSize = 40;
    this->parentsSize = 40;
    setImages("rejestracja.png", "rejestracja_ref.png");

    FunctionNode::getFunctionSet().addFunction("bitwiseNot");
    //FunctionNode::getFunctionSet().addFunction("bitwiseXor");
    //FunctionNode::getFunctionSet().addFunction("bitwiseOr");
    //FunctionNode::getFunctionSet().addFunction("bitwiseAnd");

    generator.registerObject(0.35, FunctionNode::create);
    generator.registerObject(0.65, MorphoNode::create);
}

void Application::setKatalog(string katalog)
{
    this->katalog = katalog;
}

void Application::setImages(string inputImage, string referenceImage)
{
    this->inputImage = imread(inputImage, 0);
    if( !this->inputImage.data )
    {
        string exception = "Could not open or find the input image";
        throw exception;
    }
    TerminalNode::getTerminalSet().setTerminal( MatPtr(new Mat(this->inputImage)) );
    this->referenceImage = imread(referenceImage, 0);
    if( !this->referenceImage.data )
    {
        string exception = "Could not open or find the reference image";
        throw exception;
    }
}

void Application::setParameters(int populationSize,
                                SelectionType selectType,
                                FitnessType fitType)
{
    this->populationSize = populationSize;
    this->selectType = selectType;
    this->fitType = fitType;
}

void Application::setCrossoverParameters(double subtreeCrossoverProbability,
                                         double arity2CrossoverProbability)
{
    this->subtreeCrossoverProbability = subtreeCrossoverProbability;
    this->arity2CrossoverProbability = arity2CrossoverProbability;
}

void Application::setMutationParameters(double hoistMutationProbability,
                                        double nodeMutationProbability,
                                        double collapseMutationProbability,
                                        double subtreeMutationProbability)
{
    this->hoistMutationProbability = hoistMutationProbability;
    this->nodeMutationProbability = nodeMutationProbability;
    this->collapseMutationProbability = collapseMutationProbability;
    this->subtreeMutationProbability = subtreeMutationProbability;
}

int Application::distance(const vector<Point> &a,const vector<Point> &b)
{
    int maxDistAB = 0;
    for (size_t i = 0; i < a.size(); i++)
    {
        int minB = 1000000;
        for (size_t j = 0; j < b.size(); j++)
        {
            int dx = abs(a[i].x - b[j].x);
            int dy = abs(a[i].y - b[j].y);
            int tmpDst = dx*dx + dy*dy;

            if (tmpDst < minB)
            {
                minB = tmpDst;
            }
            if (tmpDst == 0)
            {
                break;
            }
        }
        maxDistAB += minB;
    }
    if(a.size() != 0)
        maxDistAB/=a.size();
    return maxDistAB;
}

int Application::distanceHausdorff(const vector<Point> &a,const vector<Point> &b)
{
    int maxDistAB = distance(a, b);
    int maxDistBA = distance(b, a);
    int maxDist = max(maxDistAB, maxDistBA);

    return maxDist;
}

int Application::fitnessHausdorffCanny(const Mat &A, const Mat &B)
{
    vector<Point> a, b, c;
    Mat C, D;

    threshold(A, C, 125, 255, 0);
    threshold(B, D, 125, 255, 0);
    extractChannel(C, C, 0);
    extractChannel(D, D, 0);
    //bitwise_not(C,C);
    Canny(C,C,20,20*30);
    findNonZero(C, a);
    //bitwise_not(D,D);
    Canny(D,D,20,20*30);
    findNonZero(D, b);

    int measure = distanceHausdorff(a, b);
    return measure;
}

int Application::fitnessHausdorffSmall(const Mat &A, const Mat &B)
{
    vector<Point> a, b, c;
    Mat C, D;

    threshold(A, C, 125, 255, 0);
    threshold(B, D, 125, 255, 0);
    extractChannel(C, C, 0);
    extractChannel(D, D, 0);

    bitwise_not(C,C);
    resize(C,C,Size(220,114));
    findNonZero(C, a);
    bitwise_not(D,D);
    resize(D,D,Size(220,114));
    findNonZero(D, b);

    int measure = distanceHausdorff(a, b);
    return measure;
}

int Application::fitnessHamming(const Mat &A, const Mat &B)
{
    Mat C, D, E;

    threshold(A, C, 125, 255, 0);
    threshold(B, D, 125, 255, 0);
    extractChannel(C, C, 0);
    extractChannel(D, D, 0);
    bitwise_not(C, C);
    bitwise_not(D, D);
    bitwise_xor(D,C,E);
    extractChannel(E,E,0);

    int measure = 0;
    if (countNonZero(C)  < countNonZero(D))
        measure += countNonZero(E)*10;
    else measure = countNonZero(E);

    return measure;
}
    //Change with random subtree
TreePtr Application::subtreeMutation(Tree *parent)
{
    int mutationPoint = getRandomMutationPoint(parent);
    int subtreeDepth = parent->getSubtreeDepth(mutationPoint);
    Tree newSubtree(subtreeDepth, 0);
    newSubtree.initialize(FULL_INIT, generator);
    TreePtr offspring = move( parent->clone(0) );
    offspring->setSubtree(mutationPoint, *newSubtree.getRoot());

    return move(offspring);
}
    //Mutate random node
TreePtr Application::nodeMutation(Tree *parent)
{
    int mutationPoint = getRandomMutationPoint(parent);

    TreePtr offspring = move( parent->clone(0) );
    offspring->getNode(mutationPoint)->mutate();

    return move(offspring);
}
    //Offspring is subtree of parent
TreePtr Application::hoistMutation(Tree *parent)
{
    int mutationPoint = getRandomMutationPoint(parent);
    while(parent->getNode(mutationPoint)->getId().type == TERMINAL_NODE)
        mutationPoint = getRandomMutationPoint(parent);
        //Offspring is subtree with randomly picked node
    TreePtr offspring = parent->cloneSubtree(mutationPoint, 0);

    return move(offspring);
}
    //Change with terminal node
TreePtr Application::collapseMutation(Tree *parent)
{
    int mutationPoint = getRandomMutationPoint(parent);
        //Changing radomly picked node with new term node.
    NodePtr newNode( TerminalNode::create(0) );
    TreePtr offspring = move( parent->clone(0) );
    offspring->setNode(mutationPoint, *newNode);

    return move(offspring);
}

TreePtr Application::subtreeCrossover(Tree *parent1, Tree *parent2)
{
    int mutationPoint = 1;
    if (parent1->getSize() > parent2->getSize())
    {
        mutationPoint = getRandomMutationPoint(parent2);
    } else
    {
        mutationPoint = getRandomMutationPoint(parent1);
    }

    TreePtr newSubtree = move(parent1->cloneSubtree(mutationPoint, 0));
    TreePtr offspring = move( parent2->clone(0) );
    offspring->setSubtree(mutationPoint, *newSubtree->getRoot() );

    return move(offspring);
}

//TreePtr Application::arity2Crossover(Tree *parent1, Tree *parent2)
//{
//    int mutationPoint = getRandomMutationPoint(parent1);
//    TreePtr newSubtree1 = move(parent1->copySubtree(mutationPoint));

//    mutationPoint = getRandomMutationPoint(parent2);
//    TreePtr newSubtree2 = move(parent2->copySubtree(mutationPoint));

//    NodePtr newRoot( new FunctionNode(functions.getRandomFunction(2)) );
//    newRoot->addChild(newSubtree1->returnRoot());
//    newRoot->addChild(newSubtree2->returnRoot());

//    TreePtr offspring( new tree(3) );
//    offspring->setRoot( move(newRoot) );

//    return move(offspring);
//}

int Application::getRandomMutationPoint(Tree *parent)
{
    int mutationPoint = 1 + rand() % (parent->getSize() - 1);
        //Randomly picking node which isnt terminal node
    for (int i = 0; i < parent->getSize(); i++)
    {
        if ( parent->getNode(mutationPoint)->getId().type == TERMINAL_NODE )
            mutationPoint = 1 + rand() % (parent->getSize() - 1);
        else
            break;
    }
    return mutationPoint;
}


int Application::fitnessMeasure(Tree *individual)
{
    Mat image = individual->run();
    int measure = 10000;
    switch(this->fitType)
    {
    case HAMMING:
        measure = fitnessHamming(image, this->referenceImage);
        break;
    case HAUSDORFF_CANNY:
        measure = fitnessHausdorffCanny(image, this->referenceImage);
        break;
    case HAUSDORFF_SMALL:
        measure = fitnessHausdorffSmall(image, this->referenceImage);
        break;
    }

    return measure;
}

void Application::assessIndividuals()
{
    this->standardizedIndividuals.clear();
    this->normalizedIndividuals.clear();
    this->sortedIndividuals.clear();
    this->parents.clear();
    for (int i = 0; i < this->actualPopulation.getSize(); i++)
    {
        if ( this->isStopped == 1 ) return;
        Tree* individual = this->actualPopulation.getIndividual(i);
        int measure = fitnessMeasure(individual);
        pair<int,Tree*> p = make_pair(measure, individual);
        pair<double,Tree*> n = make_pair((double)measure, individual);
        this->sortedIndividuals.insert(p);
        this->parents.insert(p);
        this->normalizedIndividuals.push_back(n);
        this->standardizedIndividuals.push_back(n);
        cout<<"Oceniony osobnik numer "<<i<<endl;
        emit getIndividual(i);
    }
    normalizeFitness();
    standardizeFitness();
    pickParents();
}

void Application::pickParents()
{
        //create temporary container
    map<int,Tree*> tempMap;
        //copy 20 first element
    map<int,Tree*>::iterator end = this->parents.begin();
    for (int i = 0; i < this->parentsSize; i++)
        ++end;
    tempMap.insert(this->parents.begin(), end);
    this->parents.swap(tempMap);
}

void Application::normalizeFitness()
{
    map<int,Tree*>::iterator it =
            this->sortedIndividuals.end();
    --it;
    int worst = it->first;

    int sum = 0;
    vector<double_tree> normalizedIndividuals;
    this->normalizedIndividuals.swap(normalizedIndividuals);

    for (auto &i: normalizedIndividuals)
    {
        i.first = (worst - i.first + 1);
        sum += i.first;
    }
    double normalizedSum = 0;
    for (auto const &i: normalizedIndividuals)
    {
        double normalized = i.first / sum;
        normalizedSum += normalized;
        pair<double,Tree*> p = make_pair(normalized, i.second);
        this->normalizedIndividuals.push_back(p);
    }
}

void Application::standardizeFitness()
{
    vector<double_tree> standardizedIndividuals;
    this->standardizedIndividuals.swap(standardizedIndividuals);
    map<int,Tree*>::iterator it =
            this->sortedIndividuals.end();
    --it;
    int worst = it->first;
    for (auto const &i: standardizedIndividuals)
    {
        double standardized = i.first / worst;
        pair<double,Tree*> p = make_pair(standardized, i.second);
        this->standardizedIndividuals.push_back(p);
    }
}

Tree* Application::selectIndividual()
{
    switch(this->selectType)
    {
    case SELECTION:
        return selectIndividualFromParents();
    case ROULETTE_SELECTION:
        return selectIndividualByFitenss();
    case TOURNAMENT_SELECTION:
        return selectIndividualByTournament(this->tournamentSize);
    }
    //should never get here;
    return nullptr;
}

Tree* Application::selectIndividualFromParents()
{
    int random = rand() % this->parents.size();
    std::map<int,Tree*>::iterator it = this->parents.begin();
    for (int i = 0; i < random; i++)
        ++it;
    return it->second;
}

Tree* Application::selectIndividualByFitenss()
{
    double decision = ((double) rand() / (RAND_MAX));
    double sum = 0;
    for (auto const &p: this->normalizedIndividuals)
    {
        sum += p.first;
        if (decision <= sum)
            return p.second;
    }
    //should never get here;
    return nullptr;
}

Tree* Application::selectIndividualByTournament(int tournamentSize)
{
        //extract subvector with tournament size
     int index = rand() % (this->standardizedIndividuals.size() - tournamentSize);

     vector<double_tree>::iterator first =
             this->standardizedIndividuals.begin() + index;
     vector<double_tree>::iterator last =
             this->standardizedIndividuals.begin() + index + tournamentSize;
     vector<double_tree> tournament(first, last);
        //return best from extracted
     double_tree bestIndividual = tournament[0];
     for(auto const &it: tournament)
     {
         if (it.first < bestIndividual.first)
             bestIndividual = make_pair(it.first, it.second);
     }

     return bestIndividual.second;
}

TreePtr Application::createNewIndividual()
{
        //random for select genetic operation with different probabilities
    double random = (double)rand() / (RAND_MAX);
    double probability1 = this->subtreeCrossoverProbability;
//    double probability2 = probability1 +
//            this->arity2CrossoverProbability;
    double probability2 = probability1 +
            this->subtreeMutationProbability;
    double probability3 = probability2 +
            this->nodeMutationProbability;
    double probability4 = probability3 +
            this->hoistMutationProbability;
    double probability5 = probability4 +
            this->collapseMutationProbability;
    TreePtr newIndividual;
    if ( random < probability1 )
    {
        Tree* parent1 = selectIndividual();
        Tree* parent2 = selectIndividual();
        newIndividual = move(subtreeCrossover(parent1, parent2));

    } /*else if ( random < probability2 )
    {
        Tree* parent1 = selectIndividual();
        Tree* parent2 = selectIndividual();
        newIndividual = move(arity2Crossover(parent1, parent2));
    } */else if ( random < probability2 )
    {
        Tree* parent = selectIndividual();
        newIndividual = move(subtreeMutation(parent));
    } else if ( random < probability3 )
    {
        Tree* parent = selectIndividual();
        newIndividual = move(nodeMutation(parent));
    } else if ( random < probability4 )
    {
        Tree* parent = selectIndividual();
        newIndividual = move(hoistMutation(parent));
    } else if ( random < probability5 )
    {
        Tree* parent = selectIndividual();
        newIndividual = move(collapseMutation(parent));
    } else //if any other genetic operation just copy - reproduction
    {
        Tree* parent = selectIndividual();
        newIndividual = move(parent->clone(0));
    }

    return move(newIndividual);
}

void Application::evolution()
{
    newPopulation.clear();
    map<int,Tree*>::iterator it = this->parents.begin();
    newPopulation.addIndividual( move(it->second->clone(0)),
                                 this->generationNumber);
    for (int i = 0; i < (this->populationSize - 1); i++)
    {
        if (i == 8)
            cout<<endl;
        newPopulation.addIndividual(
                     move(createNewIndividual()),
                    this->generationNumber);
    }
    actualPopulation.swap(&newPopulation);
}

void Application::init()
{
    this->actualPopulation.create(
                this->populationSize,
                this->treeDepth,
                this->generationNumber);
    this->actualPopulation.init(generator);
}

void Application::saveBest(string &program)
{
        //write best image
    map<int,Tree*>::iterator it = this->sortedIndividuals.begin();
    int measure = it->first;
    string folder = katalog;
    Mat result = it->second->run();
    threshold(result,result,125,255,0);
    string name = "/generation";
    string nr = to_string(this->generationNumber);
    name = folder + name + nr + "_" + to_string(measure) + ".png";
    imwrite(name,result);
        //write best program
    name = "/program";
    name = folder + name + nr + "_" + to_string(measure) + ".txt";
    ofstream plik(name);
    plik << program << endl;
    plik.close();
}

void Application::stop()
{
    this->isStopped = 1;
}

void Application::run()
{
    this->generationNumber = 0;
    emit getGeneration(this->generationNumber);

    emit getOperation("Inicjacja");
    init();
    assessIndividuals();
    Tree* bestIndividual = this->sortedIndividuals.begin()->second;
    string program = bestIndividual->write();
    saveBest(program);
    //actualPopulation.savePopulation(987, this->katalog);
    checkIfBetterSolution();

    this->generationNumber++;
    emit getGeneration(this->generationNumber);

    for (int i = 0; i < this->maxGenerationNumber; i++)
    {
        if ( this->isStopped == 1 ) return;
        cout<<"::::::::::GENERACJA NR "<<this->generationNumber
              <<":::::::::::"<<endl;

        emit getOperation("Ewolucja");
        evolution();

        emit getOperation("Ocenianie");
        assessIndividuals();

        bestIndividual = this->sortedIndividuals.begin()->second;
        program = bestIndividual->write();
        saveBest(program);

        checkIfBetterSolution();
        //actualPopulation.savePopulation(this->generationNumber, this->katalog);

        if ( this->best.programResult < this->minResult )
            break;
        if ( this->best.programResult < 500 )
        {
            this->fitType = HAUSDORFF_CANNY;
            this->best.programResult = 10000000;
        }

        this->generationNumber++;
        emit getGeneration(this->generationNumber);
    }

    this->bestIndividualOutput = bestIndividual->run();
    bestIndividual = this->sortedIndividuals.begin()->second;
    program = bestIndividual->write();
    saveBest(program);
}

void Application::checkIfBetterSolution()
{
    int bestProgramResult = this->sortedIndividuals.begin()->first;
    if ( bestProgramResult < this->best.programResult )
    {
        Tree* program = this->sortedIndividuals.begin()->second;
        cout << program->write() << endl;
        int id = program->getId();
        this->bestProgram = move( program->clone(id) );
        int treeNumber = 15;

        this->best.generationNumber = this->generationNumber;
        this->best.individualNumber = treeNumber;
        this->best.image = this->bestProgram->run();
        this->best.programResult = bestProgramResult;
        emit newBestProgram(this->best);
    }
}

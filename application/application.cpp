#include "application.h"

#include <stdlib.h>
#include <QtCore>

using namespace std;
using namespace cv;

application::application()
{
    string katalog = "imgwyj2";

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
    this->populationSize = 300;
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
    setImages("imgwejsciowy1.png", "imgwyjsciowy1.png");

    functions.addFunction("bitwiseNot");
    functions.addFunction("bitwiseXor");
    functions.addFunction("bitwiseOr");
    functions.addFunction("bitwiseAnd");
    functions.addOperation("morphologyOperation");
}

void application::setKatalog(string katalog)
{
    this->katalog = katalog;
}

void application::setImages(string inputImage, string referenceImage)
{
    this->inputImage = imread(inputImage, 0);
    if( !this->inputImage.data )
    {
        string exception = "Could not open or find the input image";
        throw exception;
    }
    this->terminals.setInputImage( Mat_ptr(new Mat(this->inputImage)) );
    this->referenceImage = imread(referenceImage, 0);
    if( !this->referenceImage.data )
    {
        string exception = "Could not open or find the reference image";
        throw exception;
    }
}

void application::setParameters(int populationSize,
                                selectionType selectType,
                                fitnessType fitType)
{
    this->populationSize = populationSize;
    this->selectType = selectType;
    this->fitType = fitType;
}

void application::setCrossoverParameters(double subtreeCrossoverProbability,
                                         double arity2CrossoverProbability)
{
    this->subtreeCrossoverProbability = subtreeCrossoverProbability;
    this->arity2CrossoverProbability = arity2CrossoverProbability;
}

void application::setMutationParameters(double hoistMutationProbability,
                                        double nodeMutationProbability,
                                        double collapseMutationProbability,
                                        double subtreeMutationProbability)
{
    this->hoistMutationProbability = hoistMutationProbability;
    this->nodeMutationProbability = nodeMutationProbability;
    this->collapseMutationProbability = collapseMutationProbability;
    this->subtreeMutationProbability = subtreeMutationProbability;
}

void application::setFunctionSet(std::vector<string> functionSet)
{
    this->functions.clear();
    this->functions.addOperation("morphologyOperation");
    for (size_t i = 0; i < functionSet.size(); i++)
    {
        this->functions.addFunction(functionSet[i]);
    }
}

int application::distance(const vector<Point> &a,const vector<Point> &b)
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

int application::distanceHausdorff(const vector<Point> &a,const vector<Point> &b)
{
    int maxDistAB = distance(a, b);
    int maxDistBA = distance(b, a);
    int maxDist = max(maxDistAB, maxDistBA);

    return maxDist;
}

int application::fitnessHausdorffCanny(const Mat &A, const Mat &B)
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

int application::fitnessHausdorffSmall(const Mat &A, const Mat &B)
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

int application::fitnessHamming(const Mat &A, const Mat &B)
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
tree_ptr application::subtreeMutation(tree *parent)
{
    int mutationPoint = getRandomMutationPoint(parent);
    int subtreeDepth = parent->getSubtreeDepth(mutationPoint);
    tree_ptr newSubtree(new tree(subtreeDepth));
    newSubtree->initialize(FULL_INIT, functions, terminals);
    tree_ptr offspring = move( parent->copy() );
    offspring->setSubtree(mutationPoint, move(newSubtree->returnRoot()) );

    return move(offspring);
}
    //Change with random node
tree_ptr application::nodeMutation(tree *parent)
{
    int mutationPoint = getRandomMutationPoint(parent);
        //Create new node based on type of mutation point node
    node_ptr newNode;
    if (parent->getNode(mutationPoint)->getType() == TERMINAL_NODE)
        newNode = node_ptr( new termNode(terminals.getRandomImage()) );
    else
    {
        if (parent->getNode(mutationPoint)->getType() == FUNCTION_NODE)
        {
            node* mut = parent->getNode(mutationPoint);
            newNode = node_ptr( new funcNode(functions.getRandomFunction
                                           ( parent->getNode(mutationPoint)->getArgumentsNumber() )) );}
        else if ((parent->getNode(mutationPoint)->getType() == MORPHOLOGY_NODE))
            newNode = node_ptr( new morphoNode(functions.getRandomOperation(),
                                             functions.getRandomMorphParameters()) );
            //Copy new subtree at mutation point and make children of this subtree root
            //become children of new node
        tree_ptr newSubtree( move(parent->copySubtree(mutationPoint)) );
        for (int i = 0; i < newNode->getArgumentsNumber(); i++)
        {
            newNode->addChild( newSubtree->getRoot()->returnChild(0) );
                                     // 0 - cause returnChild() also delete this child from parent children
        }
    }
    tree_ptr offspring = move( parent->copy() );
    offspring->setSubtree(mutationPoint, move(newNode) );

    return move(offspring);
}
    //Offspring is subtree of parent
tree_ptr application::hoistMutation(tree *parent)
{
    int mutationPoint = getRandomMutationPoint(parent);
        //Offspring is subtree with randomly picked node
    tree_ptr offspring = move( parent->copySubtree(mutationPoint) );
    if ( offspring->getRoot()->getType() == TERMINAL_NODE )
        offspring = move(parent->copy());

    return move(offspring);
}
    //Change with terminal node
tree_ptr application::collapseMutation(tree *parent)
{
    int mutationPoint = getRandomMutationPoint(parent);
        //Changing radomly picked node with new term node.
    node_ptr newNode( new termNode(terminals.getRandomImage()) );
    tree_ptr offspring = move( parent->copy() );
    offspring->setSubtree(mutationPoint, move(newNode));

    return move(offspring);
}

tree_ptr application::subtreeCrossover(tree *parent1, tree *parent2)
{
    int mutationPoint = 1;
    if (parent1->getSize() > parent2->getSize())
    {
        mutationPoint = getRandomMutationPoint(parent2);
    } else
    {
        mutationPoint = getRandomMutationPoint(parent1);
    }

    tree_ptr newSubtree = move(parent1->copySubtree(mutationPoint));
    tree_ptr offspring = move( parent2->copy() );
    offspring->setSubtree(mutationPoint, move(newSubtree->returnRoot()) );

    return move(offspring);
}

tree_ptr application::arity2Crossover(tree *parent1, tree *parent2)
{
    int mutationPoint = getRandomMutationPoint(parent1);
    tree_ptr newSubtree1 = move(parent1->copySubtree(mutationPoint));

    mutationPoint = getRandomMutationPoint(parent2);
    tree_ptr newSubtree2 = move(parent2->copySubtree(mutationPoint));
    node_ptr newRoot( new funcNode(functions.getRandomFunction(2)) );
    newRoot->addChild(newSubtree1->returnRoot());
    newRoot->addChild(newSubtree2->returnRoot());

    tree_ptr offspring( new tree(3) );
    offspring->setRoot( move(newRoot) );

    return move(offspring);
}

int application::getRandomMutationPoint(tree *parent)
{
    int mutationPoint = 1 + rand() % (parent->getSize() - 1);
        //Randomly picking node which isnt terminal node
    for (int i = 0; i < parent->getSize(); i++)
    {
        if ( parent->getNode(mutationPoint)->getType() == TERMINAL_NODE )
            mutationPoint = 1 + rand() % (parent->getSize() - 1);
        else
            break;
    }
    return mutationPoint;
}


int application::fitnessMeasure(tree *individual)
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

void application::assessIndividuals()
{
    this->standardizedIndividuals.clear();
    this->normalizedIndividuals.clear();
    this->sortedIndividuals.clear();
    this->parents.clear();
    for (int i = 0; i < this->actualPopulation.getSize(); i++)
    {
        if ( this->isStopped == 1 ) return;
        tree* individual = this->actualPopulation.getIndividual(i);
        int measure = fitnessMeasure(individual);
        pair<int,tree*> p = make_pair(measure, individual);
        pair<double,tree*> n = make_pair((double)measure, individual);
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

void application::pickParents()
{
        //create temporary container
    map<int,tree*> tempMap;
        //copy 20 first element
    map<int,tree*>::iterator end = this->parents.begin();
    for (int i = 0; i < this->parentsSize; i++)
        ++end;
    tempMap.insert(this->parents.begin(), end);
    this->parents.swap(tempMap);
}

void application::normalizeFitness()
{
    map<int,tree*>::iterator it =
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
        pair<double,tree*> p = make_pair(normalized, i.second);
        this->normalizedIndividuals.push_back(p);
    }
}

void application::standardizeFitness()
{
    vector<double_tree> standardizedIndividuals;
    this->standardizedIndividuals.swap(standardizedIndividuals);
    map<int,tree*>::iterator it =
            this->sortedIndividuals.end();
    --it;
    int worst = it->first;
    for (auto const &i: standardizedIndividuals)
    {
        double standardized = i.first / worst;
        pair<double,tree*> p = make_pair(standardized, i.second);
        this->standardizedIndividuals.push_back(p);
    }
}

tree* application::selectIndividual()
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

tree* application::selectIndividualFromParents()
{
    int random = rand() % this->parents.size();
    std::map<int,tree*>::iterator it = this->parents.begin();
    for (int i = 0; i < random; i++)
        ++it;
    return it->second;
}

tree* application::selectIndividualByFitenss()
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

tree* application::selectIndividualByTournament(int tournamentSize)
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

tree_ptr application::createNewIndividual()
{
        //random for select genetic operation with different probabilities
    double random = (double)rand() / (RAND_MAX);
    double probability1 = this->subtreeCrossoverProbability;
    double probability2 = probability1 +
            this->arity2CrossoverProbability;
    double probability3 = probability2 +
            this->subtreeMutationProbability;
    double probability4 = probability3 +
            this->nodeMutationProbability;
    double probability5 = probability4 +
            this->hoistMutationProbability;
    double probability6 = probability5 +
            this->collapseMutationProbability;
    tree_ptr newIndividual;
    if ( random < probability1 )
    {
        tree* parent1 = selectIndividual();
        tree* parent2 = selectIndividual();
        newIndividual = move(subtreeCrossover(parent1, parent2));

    } else if ( random < probability2 )
    {
        tree* parent1 = selectIndividual();
        tree* parent2 = selectIndividual();
        newIndividual = move(arity2Crossover(parent1, parent2));
    } else if ( random < probability3 )
    {
        tree* parent = selectIndividual();
        newIndividual = move(subtreeMutation(parent));
    } else if ( random < probability4 )
    {
        tree* parent = selectIndividual();
        newIndividual = move(nodeMutation(parent));
    } else if ( random < probability5 )
    {
        tree* parent = selectIndividual();
        newIndividual = move(hoistMutation(parent));
    } else if ( random < probability6 )
    {
        tree* parent = selectIndividual();
        newIndividual = move(collapseMutation(parent));
    } else //if any other genetic operation just copy - reproduction
    {
        tree* parent = selectIndividual();
        newIndividual = move(parent->copy());
    }

    return move(newIndividual);
}

void application::evolution()
{
    newPopulation.clear();
    map<int,tree*>::iterator it = this->parents.begin();
    newPopulation.addIndividual( move(it->second->copy()),
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

void application::init()
{
    this->actualPopulation.create(
                this->populationSize,
                this->treeDepth,
                this->generationNumber);
    this->actualPopulation.init(functions, terminals);
}

void application::saveBest(string &program)
{
        //write best image
    map<int,tree*>::iterator it = this->sortedIndividuals.begin();
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

void application::stop()
{
    this->isStopped = 1;
}

void application::run()
{
    this->generationNumber = 0;
    emit getGeneration(this->generationNumber);

    emit getOperation("Inicjacja");
    init();
    assessIndividuals();
    tree* bestIndividual = this->sortedIndividuals.begin()->second;
    string program = bestIndividual->writeTree();
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
        program = bestIndividual->writeTree();
        saveBest(program);

        checkIfBetterSolution();
        //actualPopulation.savePopulation(this->generationNumber, this->katalog);

        if ( this->best.programResult < this->minResult ) break;
        if ( this->best.programResult < 500 )
        {
            this->fitType = HAMMING;
            this->best.programResult = 10000000;
        }

        this->generationNumber++;
        emit getGeneration(this->generationNumber);
    }

    this->bestIndividualOutput = bestIndividual->run();
}

void application::checkIfBetterSolution()
{
    int bestProgramResult = this->sortedIndividuals.begin()->first;
    if ( bestProgramResult < this->best.programResult )
    {
        tree* program = this->sortedIndividuals.begin()->second;
        string id = program->getId();
        this->bestProgram = move( program->copy(id) );
        int treeNumber = 15;

        this->best.generationNumber = this->generationNumber;
        this->best.individualNumber = treeNumber;
        this->best.image = this->bestProgram->run();
        this->best.programResult = bestProgramResult;
        emit newBestProgram(this->best);
    }
}

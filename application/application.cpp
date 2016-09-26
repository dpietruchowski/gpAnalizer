#include "application.h"

#include <stdlib.h>
#include <QtCore>
#include <iostream>
#include <tinyxml2.h>

using namespace std;
using namespace cv;
using namespace tinyxml2;

Application::Application():
    stopParam_(200, 50),
    geneticParam_(200, 5, 40, FITNESS_ROULETTESELECTION, HAMMING),
    actualPopulation_(5),
    newPopulation_(5),
    stats_(geneticParam_.populationSize),  best_()
{
    isStopped_ = 0;

    generationNumber_ = 0;
    selection_ = nullptr;

    FunctionNode::getFunctionSet().addFunction("bitwiseNot");
    FunctionNode::getFunctionSet().addFunction("bitwiseXor");
    FunctionNode::getFunctionSet().addFunction("bitwiseOr");
    FunctionNode::getFunctionSet().addFunction("bitwiseAnd");
    FunctionNode::getFunctionSet().addFunction("diff");
    FunctionNode::getFunctionSet().addFunction("recall");/*
    FunctionNode::getFunctionSet().addFunction("fillHoles");
    FunctionNode::getFunctionSet().addFunction("borderConnected");
    FunctionNode::getFunctionSet().addFunction("borderDisconnected");*/

    generator_.registerObject(0.25, FunctionNode::create);
    generator_.registerObject(0.6, MorphoNode::create);
    generator_.registerObject(0.15, ThreshNode::create);
    operationGenerator_.registerObject(0.4, SubtreeMutation::create);
    operationGenerator_.registerObject(0.05, NodeMutation::create);
    operationGenerator_.registerObject(0.03, HoistMutation::create);
    operationGenerator_.registerObject(0.02, CollapseMutation::create);
    operationGenerator_.registerObject(0.5, SubtreeCrossover::create);

    QObject::connect(&actualPopulation_, SIGNAL(getAssessedNumber(int)),
                     this, SLOT(getAssessedNumber(int)));
    QObject::connect(&actualPopulation_, SIGNAL(getBlackPixels(int)),
                     this, SLOT(getBlackPixels(int)));
    QObject::connect(&actualPopulation_, SIGNAL(getOperation(std::string)),
                     this, SLOT(setOperation(std::string)));
}

Application::Application(const Application &rhs):
    QThread(),
    katalog_(rhs.katalog_), stopParam_(rhs.stopParam_),
    referenceImage_(rhs.referenceImage_), inputImage_(rhs.inputImage_),
    geneticParam_(rhs.geneticParam_), selection_(nullptr),
    generator_(rhs.generator_), operationGenerator_(rhs.operationGenerator_),
    actualPopulation_(geneticParam_.treeDepth),
    newPopulation_(geneticParam_.treeDepth),
    stats_(geneticParam_.populationSize), best_()
{
    isStopped_ = 0;

    generationNumber_ = 0;
    selection_ = nullptr;

    QObject::connect(&actualPopulation_, SIGNAL(getAssessedNumber(int)),
                     this, SLOT(getAssessedNumber(int)));
    QObject::connect(&actualPopulation_, SIGNAL(getBlackPixels(int)),
                     this, SLOT(getBlackPixels(int)));
    QObject::connect(&actualPopulation_, SIGNAL(getOperation(std::string)),
                     this, SLOT(setOperation(std::string)));
}

Application &Application::operator =(const Application &rhs)
{
    Application copy(rhs);
    swap(copy);
    return *this;
}

void Application::swap(Application &rhs)
{
    std::swap(katalog_,rhs.katalog_);
    std::swap(isStopped_, rhs.isStopped_);
    std::swap(generationNumber_, rhs.generationNumber_);
    std::swap(stopParam_,rhs.stopParam_);
    std::swap(referenceImage_,rhs.referenceImage_);
    std::swap(inputImage_,rhs.inputImage_);
    std::swap(geneticParam_,rhs.geneticParam_);
    std::swap(generator_,rhs.generator_);
    std::swap(operationGenerator_,rhs.operationGenerator_);
    std::swap(stats_, rhs.stats_);
    std::swap(best_, rhs.best_);
    actualPopulation_.swap(&rhs.actualPopulation_);
}

Application::~Application()
{
    if(selection_ != nullptr)
    {
        delete selection_;
        selection_ = nullptr;
    }
    terminate();
    wait();
}

void Application::setKatalog(string katalog)
{
    katalog_ = katalog;
    actualPopulation_.setKatalog(katalog);
}

void Application::getKatalog(string &katalog)
{
    katalog = katalog_;
}

void Application::clearKatalog()
{
    if(katalog_ == "")
        throw string("Nie wybrano katalogu");

    string command = "rm -r  ";
    command += katalog_;
    const char *cstr = command.c_str();
    system(cstr);
    command = "mkdir ";
    command += katalog_;
    cstr = command.c_str();
    system(cstr);
}

void Application::getAssessedNumber(int number)
{
    emit getAssessed(number);
}

void Application::getBlackPixels(int pixels)
{
    stats_.addBlack(pixels);
}

void Application::setOperation(string op)
{
    emit getOperation(op);
}

void Application::setInputImage(const cv::Mat& inputImage)
{
    inputImage_ = inputImage;
    if( !inputImage_.data )
    {
        string exception = "Could not open or find the input image";
        throw exception;
    }
}

void Application::setReferenceImage(const cv::Mat& referenceImage)
{
    referenceImage_ = referenceImage;
    if( !referenceImage_.data )
    {
        string exception = "Could not open or find the reference image";
        throw exception;
    }
}

void Application::assessIndividuals()
{
    string command = "rm -r  ";
    command += katalog_ + "/population";
    const char *cstr = command.c_str();
    system(cstr);
    command = "mkdir ";
    command += katalog_ + "/population";
    cstr = command.c_str();
    system(cstr);
    if ( isStopped_ == 1 ) return;
    actualPopulation_.assess(geneticParam_.fitType, referenceImage_,
                             generator_, generationNumber_);

    if(selection_ != nullptr)
    {
        delete selection_;
        selection_ = nullptr;
    }

    switch(geneticParam_.selectType)
    {
    case FITNESS_ROULETTESELECTION:
        selection_ = new FitnessRouletteSelection();
        break;
    case RANK_ROULETTESELECTION:
        selection_ = new RankRouletteSelection();
        break;
    case TOURNAMENTSELECTION:
        selection_ = new TournamentSelection(geneticParam_.tournamentSize);
        break;
    default:
        throw string("Bad selection type");
    }

    for(int i = 0; i < actualPopulation_.getSize(); ++i)
    {
        selection_->add(actualPopulation_.getRank(i),
                       actualPopulation_.getScore(i));
        stats_.addFitness(actualPopulation_.getScore(i));
        stats_.addNNodes(actualPopulation_.getIndividual(i)->getSize());
        stats_.addDepth(actualPopulation_.getIndividual(i)->getDepth());
        stats_.addRank(actualPopulation_.getRank(i));
    }
    stats_.addMutated(actualPopulation_.getMutated());
    selection_->calcScores();
}

Tree* Application::selectIndividual()
{
    int selectedNumber = selection_->select();

    stats_.incRankSelected(actualPopulation_.getRank(selectedNumber));

    return actualPopulation_.getIndividual(selectedNumber);
}

TreePtr Application::createNewIndividual()
{
    GeneticOperation *operation = operationGenerator_.createRandomPtr();
    vector<Tree*> parents;
    for(int i = 0; i < operation->getSize(); ++i)
    {
        parents.push_back(selectIndividual());
    }


    TreePtr off = operation->reproduce(parents, &generator_);

    return move(off);
}

void Application::evolution()
{
    newPopulation_.clear();
    newPopulation_.addIndividual( actualPopulation_.getBest().second->clone(0));
    map<int,int> blackPixels;
    std::pair<std::map<int,int>::iterator,bool> ret;
    Mat result = actualPopulation_.getBest().second->run();
    int pixels = result.total() - countNonZero(result);
    blackPixels.insert(make_pair(pixels,0));
    int i = 1;
    int iterations = 0;
    int mutated = 0;
    while(newPopulation_.getSize() != actualPopulation_.getSize())
    {
        TreePtr newIndividual = createNewIndividual();
        result = newIndividual->run();
        pixels = result.total() - countNonZero(result);
        ret = blackPixels.insert(make_pair(pixels,i));
        if(ret.second == true || iterations > 100000)
        {
            emit getAssessed(i);
            newPopulation_.addIndividual( move(newIndividual) );
            i++;
            iterations = 0;
        } else
        {
            mutated++;
        }
        iterations++;
    }
    stats_.addMutated(mutated);
    actualPopulation_.swap(&newPopulation_);
}

void Application::init()
{
    actualPopulation_.create(geneticParam_.populationSize,
                             geneticParam_.treeDepth,
                             inputImage_);
    actualPopulation_.init(generator_);
}

void Application::saveBest(string &)
{
        //write best image
    pair<int, Tree*> it = actualPopulation_.getBest();
    int measure = it.first;
    string folder = katalog_;
    Mat result = it.second->run();
    threshold(result,result,125,255,0);
    string name = "/generation";
    string nr = to_string(generationNumber_);
    name = folder + name + nr + "_" + to_string(measure) + ".png";
    imwrite(name,result);
        //save xml file
    XMLDocument doc;
    XMLElement *root = it.second->save(doc);
    doc.InsertFirstChild(root);

    name = "/program";
    name = folder + name + nr + "_" + to_string(measure) + ".xml";
    XMLError eResult = doc.SaveFile(name.c_str());

        //write stats
    name = "/stats";
    name = folder + name + nr + ".m";
    stats_.save(name);

}

void Application::stop()
{
    isStopped_ = 1;
}

void Application::setGeneticOperationProbabilities(const GeneticOperationProbabilities &probabilities)
{
    if(probabilities.sum() != 1.0)
        throw string("Application::setGeneticOperationProbabilities");

    GeneticOperationGenerator generator;
    std::swap(operationGenerator_, generator);

    operationGenerator_.registerObject(probabilities.subtreeMutation,
                                    SubtreeMutation::create);
    operationGenerator_.registerObject(probabilities.nodeMutation,
                                    NodeMutation::create);
    operationGenerator_.registerObject(probabilities.hoistMutation,
                                    HoistMutation::create);
    operationGenerator_.registerObject(probabilities.collapseMutation,
                                    CollapseMutation::create);
    operationGenerator_.registerObject(probabilities.subtreeCrossover,
                                       SubtreeCrossover::create);
    operationGenerator_.registerObject(probabilities.copy,
                                       Copy::create);
}

void Application::setGeneticParameters(const GeneticParameters &param)
{
    geneticParam_ = param;
    stats_ = geneticParam_.populationSize;
}

void Application::setStopCriterium(const StopCriteriumParameters &param)
{
    stopParam_ = param;
}

void Application::setNodeProbabilities(const GeneticNodeProbabilities &probabilities)
{
    if(probabilities.sum() != 1)
        throw string("Application::setNodeProbabilities");

    NodeGenerator generator;
    std::swap(generator_, generator);

    generator_.registerObject(probabilities.function, FunctionNode::create);
    generator_.registerObject(probabilities.morpho, MorphoNode::create);
    generator_.registerObject(probabilities.thresh, ThreshNode::create);
}

void Application::run()
{
    stats_.clear();
    clearKatalog();
    generationNumber_ = 0;
    emit getGeneration(generationNumber_);

    emit getOperation("Inicjacja");
    init();
    emit getOperation("Ocenianie");
    assessIndividuals();
    Tree* bestIndividual = actualPopulation_.getBest().second;
    string program = bestIndividual->write();
    saveBest(program);
    checkIfBetterSolution();
//    actualPopulation_.savePopulation(999, katalog_);

    generationNumber_++;
    emit getGeneration(generationNumber_);
    for (int i = 0; i < stopParam_.nGenerations; i++)
    {
        stats_.clear();
        if ( isStopped_ == 1 ) return;
        cout<<":::::::::GENERACJA NR "<<generationNumber_ <<"::::::::::"<<endl;

        emit getOperation("Ewolucja");
        evolution();

        emit getOperation("Ocenianie");
        assessIndividuals();

        bestIndividual = actualPopulation_.getBest().second;
        program = bestIndividual->write();
        saveBest(program);

        checkIfBetterSolution();
//        actualPopulation_.savePopulation(generationNumber_, katalog_);

        if ( best_.programResult < stopParam_.minResult )
            break;

        generationNumber_++;
        emit getGeneration(generationNumber_);
    }

    bestIndividualOutput = bestIndividual->run();
    bestIndividual = actualPopulation_.getBest().second;
    program = bestIndividual->write();
    saveBest(program);
    emit getOperation("Koniec");
}

void Application::checkIfBetterSolution()
{
    int bestProgramResult = actualPopulation_.getBest().first;
    if ( bestProgramResult < best_.programResult )
    {
        Tree* program = actualPopulation_.getBest().second;
        cout << program->write() << endl;
        int id = program->getId();
        bestProgram_ = move( program->clone(id) );
        int treeNumber = 15;

        best_.generationNumber = generationNumber_;
        best_.individualNumber = treeNumber;
        best_.image = bestProgram_->run();
        best_.programResult = bestProgramResult;
        emit newBestProgram(best_);
    }
}

#include "population.h"

using namespace std;

population::population()
{
    this->counterTrees = 0;
}

population::population(int size, int treeDepth, int generationNumber)
{
    this->counterTrees = 0;
    for(int i=0; i<size; i++)
    {
        string id = 'p' + to_string(generationNumber) + 't' + to_string(counterTrees++);
        individuals.push_back( tree_ptr(new tree(treeDepth, id)) );
    }
}

population::~population()
{
}

void population::erase(int i)
{
    vector<tree_ptr>::iterator it = individuals.begin();
    for (int n = 0; n < i; n++)
        ++it;
    individuals.erase(it);
}

void population::create(int size, int treeDepth, int generationNumber)
{
    this->counterTrees = 0;
    for(int i=0; i<size; i++)
    {
        string id = 'p' + to_string(generationNumber) + 't' + to_string(counterTrees++);
        individuals.push_back( tree_ptr(new tree(treeDepth, id)) );
    }
}

void population::init(const functionSet &functions, const terminalSet &terminals)
{
    int populationSize = this->getSize();
    for (initializeIt=individuals.begin(); initializeIt<individuals.begin()+populationSize/2; ++initializeIt)
    {
       (*initializeIt)->initialize(FULL_INIT, functions, terminals);
    }

    for (; initializeIt<individuals.end(); ++initializeIt)
    {
       (*initializeIt)->initialize(GROW_INIT, functions, terminals);
    }
}

tree* population::getIndividual(int i)
{
    return individuals[i].get();
}

void population::addIndividual(tree_ptr newIndividual, int generationNumber)
{
    string id = 'p' + to_string(generationNumber) + 't' + to_string(counterTrees++);
    newIndividual->setId(id);
    individuals.push_back( move(newIndividual) );
}

int population::getSize()
{
    return individuals.size();
}

void population::swap(population* newPopulation)
{
    this->individuals.swap(newPopulation->individuals);
}

void population::clear()
{
    individuals.clear();
    this->counterTrees = 0;
}

void population::show()
{
    for (vector<tree_ptr>::iterator it=individuals.begin(); it!=individuals.end(); ++it)
    {
        (*it)->show();
    }
}

void population::savePopulation(int generationNumber, string katalog)
{
    string command = "mkdir ";
    command += katalog + "/population";
    command += to_string(generationNumber);
    const char *cstr = command.c_str();
    system(cstr);
    int i = 0;
    for(auto const &p: this->individuals)
    {
        Mat result = p->run();
        string folder = "images/population";
        string nr = to_string(generationNumber);
        string name = "individual";
        name = folder + nr + "/" + name + to_string(i) + ".png";
        imwrite(name,result);
        i++;
    }
}

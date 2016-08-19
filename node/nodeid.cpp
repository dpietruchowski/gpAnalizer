#include "nodeid.h"

#include <utility>
#include <iterator>

using namespace std;

NodeId::NodeId():
    type(NULL_NODE), number(0), cloneNumber(0)
{
}

NodeId::NodeId(NodeType type, unsigned int number, unsigned int cloneNumber):
    type(type), number(number), cloneNumber(cloneNumber)
{
}

void NodeId::swap(NodeId &rhs)
{
    std::swap(type, rhs.type);
    std::swap(number, rhs.number);
    std::swap(cloneNumber, rhs.cloneNumber);
}

bool NodeId::operator ==(const NodeId &rhs) const
{
    return (type == rhs.type)
            && (number == rhs.number)
            && (cloneNumber == rhs.cloneNumber);
}

string NodeId::toString() const
{
    string id = enumToString(type);
    id += " ";
    id += to_string(number);
    id += " ";
    id += to_string(cloneNumber);

    return id;
}

// Format: "NULL_NODE 6516 20"
void NodeId::fromString(const string &id)
{
    size_t firstSpace = id.find_first_of(" ");
    size_t secondSpace = id.find_last_of(" ");
    string stype = id.substr(0, firstSpace);
    type = enumFromString(stype);
    string snumber = id.substr(firstSpace+1, secondSpace - firstSpace);
    number = stoi(snumber);
    string sclone = id.substr(secondSpace+1);
    cloneNumber = stoi(sclone);
}


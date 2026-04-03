#include "nodeid.h"

#include <utility>
#include <iterator>

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

std::string NodeId::toString() const
{
    std::string id = enumToString(type);
    id += " ";
    id += std::to_string(number);
    id += " ";
    id += std::to_string(cloneNumber);

    return id;
}

// Format: "NULL_NODE 6516 20"
void NodeId::fromString(const std::string &id)
{
    size_t firstSpace = id.find_first_of(" ");
    size_t secondSpace = id.find_last_of(" ");
    std::string stype = id.substr(0, firstSpace);
    type = enumFromString(stype);
    std::string snumber = id.substr(firstSpace+1, secondSpace - firstSpace);
    number = std::stoi(snumber);
    std::string sclone = id.substr(secondSpace+1);
    cloneNumber = std::stoi(sclone);
}

void NodeId::saveAttribute(tinyxml2::XMLElement *node) const
{
    node->SetAttribute("type", enumToString(type));
    node->SetAttribute("number", number);
    node->SetAttribute("cloneNumber", cloneNumber);
}

void NodeId::loadAttribute(const tinyxml2::XMLElement *node)
{
    const char* typeStr = node->Attribute("type");
    type = enumFromString(std::string(typeStr));
    number = node->IntAttribute("number");
    cloneNumber = node->IntAttribute("cloneNumber");
}


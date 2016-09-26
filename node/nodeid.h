#ifndef NODEID_H
#define NODEID_H

#include <boost/preprocessor.hpp>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <functional>
#include <algorithm>
#include <tinyxml2.h>

#define X_DEFINE_ENUM_TOSTRING_CASE(r, data, elem)    \
case elem : return BOOST_PP_STRINGIZE(elem);

#define DEFINE_ENUM(name, enumerators)                \
enum  name {                                                               \
    BOOST_PP_SEQ_ENUM(enumerators)                                        \
};                                                                        \
                                                                          \
inline  const char* enumToString(name v)                                       \
{                                                                         \
    switch (v)                                                            \
    {                                                                     \
        BOOST_PP_SEQ_FOR_EACH(                                            \
            X_DEFINE_ENUM_TOSTRING_CASE,          \
            name,                                                         \
            enumerators                                                   \
        )                                                                 \
        default: return "[Unknown " BOOST_PP_STRINGIZE(name) "]";         \
    }                                                                     \
}

DEFINE_ENUM(NodeType,(NULL_NODE)(TERMINAL_NODE)(FUNCTION_NODE)            \
            (MORPHO_NODE)(THRESH_NODE))

template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6)
{
    std::ostringstream out;
    out << std::setprecision(n) << a_value;
    return out.str();
}

inline NodeType enumFromString(const std::string& enm)
{
    if(enm == "NULL_NODE") return NULL_NODE;
    if(enm == "TERMINAL_NODE") return TERMINAL_NODE;
    if(enm == "FUNCTION_NODE") return FUNCTION_NODE;                   \
    if(enm == "MORPHO_NODE") return MORPHO_NODE;                 \
    if(enm == "THRESH_NODE") return THRESH_NODE;

    throw std::string("Wrong enum");
}

struct NodeId
{
    NodeType type;
    unsigned int number;
    unsigned int cloneNumber;
    NodeId();
    NodeId(NodeType type, unsigned int number, unsigned int cloneNumber);
    void swap(NodeId& rhs);
    bool operator ==(const NodeId& rhs) const;
    std::string toString() const;
    void fromString(const std::string& id);
    void saveAttribute(tinyxml2::XMLElement *node) const;
    void loadAttribute(const tinyxml2::XMLElement *node);
};

#endif // NODEID_H

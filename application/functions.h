#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>

template <class T>
std::string to_string(T t)
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}

#endif // FUNCTIONS_H

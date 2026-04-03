#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>

class GpException : public std::runtime_error
{
public:
    explicit GpException(const std::string& message)
        : std::runtime_error(message) {}
};

class InvalidArgumentException : public GpException
{
public:
    explicit InvalidArgumentException(const std::string& message)
        : GpException(message) {}
};

class InvalidEnumException : public GpException
{
public:
    explicit InvalidEnumException(const std::string& message)
        : GpException(message) {}
};

class ParseException : public GpException
{
public:
    explicit ParseException(const std::string& message)
        : GpException(message) {}
};

class ImageException : public GpException
{
public:
    explicit ImageException(const std::string& message)
        : GpException(message) {}
};

class ConfigurationException : public GpException
{
public:
    explicit ConfigurationException(const std::string& message)
        : GpException(message) {}
};

#endif // EXCEPTIONS_H

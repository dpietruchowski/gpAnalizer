#ifndef TERMINALSET_H
#define TERMINALSET_H

#include <memory>
#include <opencv2/core/core.hpp>

typedef std::shared_ptr<cv::Mat> MatPtr;

class TerminalSet
{
public:
    TerminalSet(MatPtr terminal);
    MatPtr getTerminal() const;
    void setTerminal(MatPtr terminal);
private:
    MatPtr terminal_;
};

#endif // TERMINALSET_H

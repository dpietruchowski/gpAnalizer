#include "terminalset.h"

TerminalSet::TerminalSet(MatPtr terminal):
    terminal_(terminal)
{
}

MatPtr TerminalSet::getTerminal() const
{
    return terminal_;
}

void TerminalSet::setTerminal(MatPtr terminal)
{
    terminal_ = terminal;
}

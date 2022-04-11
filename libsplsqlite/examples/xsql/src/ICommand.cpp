#include "xsql/ICommand.h"

ICommand::~ICommand()
{
}

#ifdef DEBUG
void ICommand::ValidateMem() const
{
}

void ICommand::CheckMem() const
{
}
#endif

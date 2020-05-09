#ifndef SMASH_COMMAND_H_
#define SMASH_COMMAND_H_

#include <string>
#include <time.h>
#include <list>
#include <vector>

#include "signals.h"
#include "SmallShell.h"

class Command {
public:
    Command(const char* cmd_line);
    virtual ~Command();
    virtual void execute() = 0;
    //virtual void prepare();
    //virtual void cleanup();
};

#endif //SMASH_COMMAND_H_

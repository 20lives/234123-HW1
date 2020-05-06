//
// Created by nomi on 5/6/20.
//
#ifndef HW1_234123_EXTERNALCOMMAND_H
#define HW1_234123_EXTERNALCOMMAND_H

#include "Commands.h"
#include "Utilities.h"

/**
 * BuiltInCommand class
 */
class ExternalCommand : public Command {
protected:
    char cmdLine[COMMAND_ARGS_MAX_LENGTH] = {};
    int cmdLineLength = 0;
    pid_t pid;
public:
    bool isBackgroundCmd;
    ExternalCommand(const char* cmd_line, bool isBackgroundCmd);
    virtual ~ExternalCommand() {}
    void execute() override;
    void setPid(pid_t _pid);
    pid_t getPid();
};

#endif //HW1_234123_EXTERNALCOMMAND_H

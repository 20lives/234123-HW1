//
// Created by nomi on 5/9/20.
//

#ifndef HW1_234123_PIPECOMMAND_H
#define HW1_234123_PIPECOMMAND_H

#include "Commands.h"

class PipeCommand : public Command {
    // TODO: Add your data members
    char rawCmdLine[COMMAND_ARGS_MAX_LENGTH] = {};
    int cmdLineLength = 0;
    bool isBackground = false;
    bool isErr = false;
public:
    PipeCommand(const char* cmd_line, bool _isBackground, bool _isErr = false);
    virtual ~PipeCommand() {}
    void execute() override;
};

#endif //HW1_234123_PIPECOMMAND_H

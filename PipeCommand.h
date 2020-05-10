//
// Created by nomi on 5/9/20.
//

#ifndef HW1_234123_PIPECOMMAND_H
#define HW1_234123_PIPECOMMAND_H

#include "Commands.h"

class PipeInfo {
    PipeInfo(){

    }
public:
    int origin_out;
    int origin_err;
    bool isBackground = false;
    bool isErr = false;
    bool isPipe = false;
    int pid = -1;
    char rawCmdLine[COMMAND_ARGS_MAX_LENGTH] = {};
    static PipeInfo& getInstance() {
        static PipeInfo instance;
        return instance;
    }
};


class PipeCommand : public Command {
    // TODO: Add your data members
    // TODO: Add your data members
    char rawCmdLine[COMMAND_ARGS_MAX_LENGTH] = {};
    int cmdLineLength = 0;
    bool isBackground = false;
    bool isErr = false;

    char firstCmd[COMMAND_ARGS_MAX_LENGTH] = {};
    char secondCmd[COMMAND_ARGS_MAX_LENGTH] = {};
public:
    PipeCommand(const char* cmd_line, bool _isBackground, bool _isErr = false);
    virtual ~PipeCommand() {}
    void execute() override;
};

#endif //HW1_234123_PIPECOMMAND_H

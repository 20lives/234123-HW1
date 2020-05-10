//
// Created by nomi on 5/9/20.
//

#ifndef HW1_234123_REDIRECTIONCOMMAND_H
#define HW1_234123_REDIRECTIONCOMMAND_H
#include "Utilities.h"

class RedInfo {
    RedInfo(){

    }
public:
    int origin_out;
    bool isBackground = false;
    bool isRedirection = false;
    char rawCmdLine[COMMAND_ARGS_MAX_LENGTH] = {};
    static RedInfo& getInstance() {
        static RedInfo instance;
        return instance;
    }
};


#include "Commands.h"

class RedirectionCommand : public Command {
    // TODO: Add your data members
    char rawCmdLine[COMMAND_ARGS_MAX_LENGTH] = {};
    int cmdLineLength = 0;
    bool isBackground = false;
    bool isAppend = false;

    char cmd[COMMAND_ARGS_MAX_LENGTH] = {};
    char file[COMMAND_ARGS_MAX_LENGTH] = {};
public:
    explicit RedirectionCommand(const char* cmd_line, bool _isRedirectionCmd, bool isAppend = false);
    virtual ~RedirectionCommand() {}
    void execute() override;
    //void prepare() override;
    //void cleanup() override;
};

#endif //HW1_234123_REDIRECTIONCOMMAND_H

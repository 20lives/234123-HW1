//
// Created by nomi on 5/6/20.
//
#ifndef HW1_234123_BUILTINCOMMAND_H
#define HW1_234123_BUILTINCOMMAND_H

#include "Commands.h"
#include "Utilities.h"

/**
 * BuiltInCommand class
 */
class BuiltInCommand : public Command {
protected:
    char *argv[COMMAND_MAX_ARGS] = {};
    int argc;
public:
    BuiltInCommand(const char* cmd_line);
    virtual ~BuiltInCommand();
    char *getArg(int i);
    int getArgCount();
};

/**
 * classes that inherit BuiltInCommand class
 */
class ChangePromptCommand : public BuiltInCommand {
public:
    ChangePromptCommand(const char* cmd_line);
    virtual ~ChangePromptCommand() {}
    void execute() override;
};

class ShowPidCommand : public BuiltInCommand {
public:
    ShowPidCommand(const char* cmd_line);
    virtual ~ShowPidCommand() {}
    void execute() override;
};

class PwdCommand : public BuiltInCommand {
public:
    PwdCommand(const char* cmd_line);
    virtual ~PwdCommand() {}
    void execute() override;
};

class CdCommand : public BuiltInCommand {
public:
    CdCommand(const char* cmd_line);
    virtual ~CdCommand() {}
    void execute() override;
};

class JobsList;
class QuitCommand : public BuiltInCommand {
    // TODO: Add your data members public:
    QuitCommand(const char* cmd_line, JobsList* jobs);
    virtual ~QuitCommand() {}
    void execute() override;
};

class HistoryCommand : public BuiltInCommand {
// TODO: Add your data members
public:
    HistoryCommand(const char* cmd_line, CommandsHistory* history);
    virtual ~HistoryCommand() {}
    void execute() override;
};

class JobsCommand : public BuiltInCommand {
// TODO: Add your data members
public:
    JobsCommand(const char* cmd_line, JobsList* jobs);
    virtual ~JobsCommand() {}
    void execute() override;
};

class KillCommand : public BuiltInCommand {
    // TODO: Add your data members
public:
    KillCommand(const char* cmd_line, JobsList* jobs);
    virtual ~KillCommand() {}
    void execute() override;
};

class ForegroundCommand : public BuiltInCommand {
// TODO: Add your data members
public:
    ForegroundCommand(const char* cmd_line, JobsList* jobs);
    virtual ~ForegroundCommand() {}
    void execute() override;
};

class BackgroundCommand : public BuiltInCommand {
// TODO: Add your data members
public:
    BackgroundCommand(const char* cmd_line, JobsList* jobs);
    virtual ~BackgroundCommand() {}
    void execute() override;
};

// TODO: should it really inhirit from BuiltInCommand ?
class CopyCommand : public BuiltInCommand {
public:
    CopyCommand(const char* cmd_line);
    virtual ~CopyCommand() {}
    void execute() override;
};

#endif //HW1_234123_BUILTINCOMMAND_H

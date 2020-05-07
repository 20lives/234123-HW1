//
// Created by nomi on 5/6/20.
//
#include <unistd.h>
#include <string.h>
#include <vector>

#include "SmallShell.h"
#include "Utilities.h"
#include "Commands.h"
#include "BuiltInCommand.h"
#include "ExternalCommand.h"

/**
 * Aux functions
 */
void _setCmdType(char *cmd, CMD_TYPE *eCmdType) {
    if (strcmp(cmd, "chprompt") == 0) {
        *eCmdType = eChprompt;
    } else if (strcmp(cmd, "showpid") == 0) {
        *eCmdType = eShowpid;
    } else if (strcmp(cmd, "pwd") == 0) {
        *eCmdType = ePwd;
    } else if (strcmp(cmd, "cd") == 0) {
        *eCmdType = eCd;
    } else if ((strcmp(cmd, "kill") == 0)){
        *eCmdType = eKill;
    } else if ((strcmp(cmd, "fg") == 0)){
        *eCmdType = eFg;
    } else if ((strcmp(cmd, "bg") == 0)){
        *eCmdType = eBg;
    } else if ((strcmp(cmd, "quit") == 0)){
        *eCmdType = eQuit;
    } else {
        *eCmdType = eExternal;
    }
}

/**
 * Constructors and destructors
 */
SmallShell::SmallShell() {
}

SmallShell::~SmallShell() {
// TODO: add your implementation
}

/**
 * getters and setters
 */
void SmallShell::setPrompt(const char *_prompt) {
    prompt = _prompt;
}

string SmallShell::getPrompt() {
    return prompt;
}

void SmallShell::setPrevDir(const char* _dir) {
    prevDir = _dir;
}

string SmallShell::getPrevDir() {
    return prevDir;
}

/**
 * other class methods
 */

/**
* Creates and returns a pointer to Command class which matches the given command line (cmd_line)
*/
Command *SmallShell::CreateCommand(const char* cmd_line) {
    char *argv[COMMAND_MAX_ARGS] = {};
    int argc = _parseCommandLine(cmd_line, argv);

    if (argc == 0) {
        _freeFields(argv, argc);
        return nullptr;
    }
    char cmdLine[COMMAND_ARGS_MAX_LENGTH];
    strcpy(cmdLine, cmd_line);
    bool isBackgroundCmd;
    CMD_TYPE eCmdType;

    // 1. check if &
    isBackgroundCmd = (string(cmd_line).back() == '&');
    // 2. remove last &
    _removeBackgroundSign(cmdLine);
    // now we have the cmd line with out the last & and we have a boolean value indicating if this is a background cmd
    // 3. set cmd type
    _setCmdType(argv[0], &eCmdType);
    // 4. free the cmdLine
    _freeFields(argv, argc);
    // 5. create te command
    if (eCmdType == eChprompt) {
        return new ChangePromptCommand(cmdLine);
    } else if (eCmdType == eShowpid) {
        return new ShowPidCommand(cmdLine);
    } else if (eCmdType == ePwd) {
        return new PwdCommand(cmdLine);
    } else if (eCmdType == eCd) {
        return new CdCommand(cmdLine);
    } else if (eCmdType == eKill) {
        return new KillCommand(cmdLine);
    } else if (eCmdType == eFg) {
        return new ForegroundCommand(cmdLine);
    } else if (eCmdType == eBg) {
        return new BackgroundCommand(cmdLine);
    } else if (eCmdType == eQuit) {
        return new QuitCommand(cmdLine);
    } else {
        // external cmd is the last possibly
        return new ExternalCommand(cmdLine, isBackgroundCmd);
    }
    return nullptr;
}

void SmallShell::executeCommand(const char *cmd_line) {
    // TODO: Add your implementation here

    // for example:
    Command* cmd = CreateCommand(cmd_line);
    if (cmd) {
        cmd->execute();
    }
    delete(cmd);
    // Please note that you must fork smash process for some commands (e.g., external commands....)
};

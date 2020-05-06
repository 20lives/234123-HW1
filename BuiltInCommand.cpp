//
// Created by nomi on 5/6/20.
//

#include <unistd.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <sys/wait.h>
#include <iomanip>
#include <errno.h>

#include "BuiltInCommand.h"
#include "Commands.h"
#include "Utilities.h"

using namespace std;

enum CD_TYPE {
    ePrevDir, eChangeDir
};

/**
 * Aux functions
 */
void  _changeDirectory(CD_TYPE cdType, char* dir = nullptr) {
    SmallShell& smash = SmallShell::getInstance();
    char currWorkingDir[COMMAND_ARGS_MAX_LENGTH] = {};
    char newWorkingDir[COMMAND_ARGS_MAX_LENGTH] = {};
    getcwd(currWorkingDir, COMMAND_ARGS_MAX_LENGTH);
    if (cdType == ePrevDir) {
        if (!smash.isPrevDirSet) {
            std::cout << "smash error: cd: OLDPWD not set" << "\n";
            return;
        }
        strcpy(newWorkingDir, smash.getPrevDir().c_str());
    } else {
        strcpy(newWorkingDir, dir);
    }
    if (chdir(newWorkingDir) == -1) {
        //chdir failed
        perror("smash error: cd failed");
    }
    smash.setPrevDir(currWorkingDir);
    smash.isPrevDirSet = true;
}

/**
 * Constructors and destructors
 */
BuiltInCommand::BuiltInCommand(const char* cmd_line) : Command(cmd_line) {
    argc = _parseCommandLine(cmd_line, argv);
}

ChangePromptCommand::ChangePromptCommand(const char* cmd_line) : BuiltInCommand(cmd_line) {

}

ShowPidCommand::ShowPidCommand(const char* cmd_line) : BuiltInCommand(cmd_line) {

}

PwdCommand::PwdCommand(const char* cmd_line) : BuiltInCommand(cmd_line) {

}

CdCommand::CdCommand(const char* cmd_line) : BuiltInCommand(cmd_line) {

}

BuiltInCommand::~BuiltInCommand() {
    _freeFields(argv, argc);
}

/**
 * class execution methods
 */
void ChangePromptCommand::execute() {
    SmallShell& smash = SmallShell::getInstance();
    char prompt[COMMAND_ARGS_MAX_LENGTH] = {};
    if (getArgCount() > 1) {
        strcpy(prompt, getArg(1));
        smash.setPrompt(prompt);
    } else {
        smash.setPrompt("smash");
    }
}

void ShowPidCommand::execute() {
    std::cout << "smash pid is " << getpid() << "\n";
}

void PwdCommand::execute() {
    char currWorkingDir[COMMAND_ARGS_MAX_LENGTH];
    getcwd(currWorkingDir, COMMAND_ARGS_MAX_LENGTH);
    std::cout << currWorkingDir << "\n";
}

void CdCommand::execute() {
    if (getArgCount() > 2) {
        std::cout << "smash error: cd: too many arguments" << "\n";
    } else if (getArgCount() == 2) {
        if (strcmp(getArg(1), "-") == 0) {
            _changeDirectory(ePrevDir);
        } else {
            _changeDirectory(eChangeDir, getArg(1));
        }
    }
}

/**
 * getters and setters
 */
char* BuiltInCommand::getArg(int i) {
    return argv[i];
}

int BuiltInCommand::getArgCount() {
    return argc;
}
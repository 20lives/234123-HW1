//
// Created by nomi on 5/9/20.
//
#include <string.h>

#include <string.h>
#include <iostream>

#include "RedirectionCommand.h"
#include "Utilities.h"

RedirectionCommand::RedirectionCommand(const char *cmd_line, bool _isBackground, bool _isAppend) : Command(cmd_line) {
    strcpy(rawCmdLine, cmd_line);
    isBackground = _isBackground;
    isAppend = _isAppend;

    char cmdLine[COMMAND_ARGS_MAX_LENGTH] = {};
    strcpy(cmdLine, cmd_line);
    _removeBackgroundSign(cmdLine);

    std::size_t found = string(cmd_line).find_first_of('>');
    if (!_isAppend) {
        strcpy(cmd, string(cmdLine).substr(0, found).c_str());
        strcpy(file, string(cmdLine).substr(found + 1).c_str());
    } else {
            strcpy(cmd, string(cmdLine).substr(0, found).c_str());
            strcpy(file, string(cmdLine).substr(found + 2).c_str());
    }
    std::cout << "cmd: " << cmd << "file: " << file << '\n';

}

void RedirectionCommand::execute() {

}


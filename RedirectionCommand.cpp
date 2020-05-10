//
// Created by nomi on 5/9/20.
//
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>

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
    strcpy(cmd, string(cmdLine).substr(0, found).c_str());
    strcpy(file, _trim(string(cmdLine).substr(found + 1 + _isAppend)).c_str());
}

void RedirectionCommand::execute() {
    SmallShell& smallShell = SmallShell::getInstance();
    RedInfo& redInfo = RedInfo::getInstance();
    redInfo.isBackground = this->isBackground;
    redInfo.isRedirection = true;
    strcpy(redInfo.rawCmdLine, rawCmdLine);

    int origin_out = dup(1);
    redInfo.origin_out = origin_out;
    int fd_out = (!isAppend) ? open(file,  O_WRONLY|O_CREAT|O_TRUNC,0666) : open(file, O_WRONLY|O_CREAT|O_TRUNC | O_APPEND,0666);
    close(1);
    dup(fd_out);
    close(fd_out);

    smallShell.executeCommand(cmd);

    close(1);
    dup(origin_out);
    close(origin_out);
    redInfo.isBackground = false;
    redInfo.isRedirection = false;
}


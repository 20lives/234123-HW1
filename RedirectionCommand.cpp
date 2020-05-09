//
// Created by nomi on 5/9/20.
//
#include <string.h>

#include "RedirectionCommand.h"
#include "Utilities.h"

RedirectionCommand::RedirectionCommand(const char *cmd_line, bool _isBackground, bool _isAppend) : Command(cmd_line) {
    strcpy(rawCmdLine, cmd_line);
    isBackground = _isBackground;
    isAppend = _isAppend;
}

void RedirectionCommand::execute() {

}


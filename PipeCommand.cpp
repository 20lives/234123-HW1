//
// Created by nomi on 5/9/20.
//

#include <string.h>
#include <iostream>

#include "Utilities.h"
#include "PipeCommand.h"

PipeCommand::PipeCommand(const char *cmd_line, bool _isBackground, bool _isErr) : Command(cmd_line) {
    strcpy(rawCmdLine, cmd_line);
    isBackground = _isBackground;
    isErr = _isErr;
}

void PipeCommand::execute() {

}
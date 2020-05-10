//
// Created by nomi on 5/6/20.
//

#include <unistd.h>
#include <string.h>
#include <vector>
#include <sys/wait.h>

#include "ExternalCommand.h"
#include "Utilities.h"
#include "Commands.h"
#include "JobsList.h"
#include "FgJob.h"
#include "RedirectionCommand.h"

/**
 * Constructors and destructors
 */
ExternalCommand::ExternalCommand(const char* cmd_line, bool _isBackgroundCmd) : isBackgroundCmd(_isBackgroundCmd), Command(cmd_line) {
    strcpy(rawCmdLine, cmd_line);
    strcpy(cmdLine, cmd_line);
    _removeBackgroundSign(cmdLine);
    cmdLineLength = string(cmd_line).length();
}

/**
 * class execution methods
 */
void ExternalCommand::execute() {
    // update args array
    char *args[4] = {};
    args[0] = (char*)malloc(string("/bin/bash").length() + 1);
    strcpy(args[0], "/bin/bash");
    args[1] = (char*)malloc(string("-c").length() + 1);
    strcpy(args[1], "-c");
    args[2] = (char*)malloc(cmdLineLength + 1);
    strcpy(args[2], (string(cmdLine)).c_str());
    args[3] = NULL;

    pid_t pid = fork();
    if (pid == 0) {
        setpgrp();
        execv("/bin/bash", args);
        exit(0);
    } else {
        // parent pid
        if (!isBackgroundCmd){
            FgJob& fgJob = FgJob::getInstance();
            fgJob.updateFg(rawCmdLine, pid);
            // in case this is not a background cmd we will wait for the child process to finish
            waitpid(pid, NULL, WUNTRACED);
        } else {
            // running in the background initially
            RedInfo& redInfo = RedInfo::getInstance();
            JobsList& list = JobsList::getInstance();
            if (!redInfo.isRedirection) {
            list.addJob(rawCmdLine, pid);
            } else {
                list.addJob(redInfo.rawCmdLine, pid);
            }
        }
    }
}

/**
 * getters and setters
 */

string ExternalCommand::getCommandLine() {
    return cmdLine;
}

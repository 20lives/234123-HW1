//
// Created by nomi on 5/8/20.
//
#include <string.h>

#include "FgJob.h"

FgJob::FgJob() {

}

FgJob::~FgJob() {

}

void FgJob::updateFg(const char *_cmdLine, pid_t _pid) {
    isFgJob = true;
    strcpy(cmdLine, _cmdLine);
    pid = _pid;
}

pid_t FgJob::getPid() {
    return pid;
}

void FgJob::clearFg() {
    isFgJob = false;
    memset(cmdLine, 0, 255);
    cmdLineLength = 0;
    pid = -1;
}

bool FgJob::isFgJobRunning() {
    return isFgJob;
}

const char *FgJob::getCmdLine() {
    return cmdLine;
}

//
// Created by nomi on 5/8/20.
//

#ifndef HW1_234123_FGJOB_H
#define HW1_234123_FGJOB_H

#include "Utilities.h"

class FgJob {
private:
    bool isFgJob = false;

    char cmdLine[COMMAND_ARGS_MAX_LENGTH] = {};
    int cmdLineLength = 0;

    pid_t pid = -1;

    FgJob();
public:
    FgJob(FgJob const&) = delete;
    void operator=(FgJob const&) = delete;
    static FgJob& getInstance() {
        static FgJob instance;
        return instance;
    }
    void updateFg(const char* _cmdLine, pid_t _pid);
    void clearFg();
    pid_t getPid();
    const char* getCmdLine();
    bool isFgJobRunning();
    ~FgJob();
};


#endif //HW1_234123_FGJOB_H

//
// Created by nomi on 5/6/20.
//

#include <unistd.h>
#include <string.h>
#include <iostream>
#include <signal.h>

#include "BuiltInCommand.h"
#include "Commands.h"
#include "JobsList.h"
#include "Utilities.h"

using namespace std;

enum CD_TYPE {
    ePrevDir, eChangeDir
};

/**
 * Aux functions
 */
int _strToInt(char *arg) {
    return atoi(arg);
}

bool _isInteger(char *arg) {
    string s = string(arg);
    if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;
    char * p;
    strtol(s.c_str(), &p, 10);
    return (*p == 0);
}

bool _handleArgsForKillCmd(char **argv, int argc) {
    // the kill cmd should be as following: kill -<num> <num>
    bool isValidArgs = true;
    if (argc != 3) {
        isValidArgs = false;
    }
    if (isValidArgs && (string(argv[1]).length() <= 1)) {
        isValidArgs = false;
    }
    if (isValidArgs && argv[1][0] != '-') {
        isValidArgs = false;
    }
    // erase the '-' char
    if (isValidArgs) {
        memmove(argv[1], string(argv[1]).erase(0, 1).c_str(), sizeof(argv[1] - 1));
        if (!_isInteger(argv[1]) || !_isInteger(argv[2])) {
            isValidArgs = false;
        }
    }
    if (!isValidArgs) {
        std::cerr << "smash error: kill: invalid arguments" << "\n";
    }
    return isValidArgs;
}

void  _changeDirectory(CD_TYPE cdType, char* dir = nullptr) {
    SmallShell& smash = SmallShell::getInstance();
    char currWorkingDir[COMMAND_ARGS_MAX_LENGTH] = {};
    char newWorkingDir[COMMAND_ARGS_MAX_LENGTH] = {};
    getcwd(currWorkingDir, COMMAND_ARGS_MAX_LENGTH);
    if (cdType == ePrevDir) {
        if (!smash.isPrevDirSet) {
            std::cerr << "smash error: cd: OLDPWD not set" << "\n";
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

KillCommand::KillCommand(const char* cmd_line) : BuiltInCommand(cmd_line) {

}

ForegroundCommand::ForegroundCommand(const char *cmd_line) : BuiltInCommand(cmd_line){

}

BackgroundCommand::BackgroundCommand(const char *cmd_line) : BuiltInCommand(cmd_line){

}

QuitCommand::QuitCommand(const char *cmd_line) : BuiltInCommand(cmd_line){

}

JobsCommand::JobsCommand(const char *cmd_line) : BuiltInCommand(cmd_line) {

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
        std::cerr << "smash error: cd: too many arguments" << "\n";
    } else if (getArgCount() == 2) {
        if (strcmp(getArg(1), "-") == 0) {
            _changeDirectory(ePrevDir);
        } else {
            _changeDirectory(eChangeDir, getArg(1));
        }
    }
}

void KillCommand::execute() {
    bool isValidArgs = _handleArgsForKillCmd(argv, argc);
    JobsList& jobsList = JobsList::getInstance();
    if (isValidArgs) {
        int sig = _strToInt(argv[2]);
        int jobId = _strToInt(argv[2]);
        pid_t jobPid = jobsList.getJobPid(jobId);
        // assuming getJobById is returning NULL if there is no job associated with the same id
        if (jobPid == -1) {
            std::cerr << "smash error: kill: job-id " << jobId << " does not exist" << "\n";
            return;
        }
        // send signal to process
        kill(jobPid, sig);
        std::cout << "signal number " << sig << " was sent to pid " << jobPid << "\n";
    }
}

void ForegroundCommand::execute() {
    if (argc > 2 || argv[1] && !_isInteger(argv[1])) {
        std::cerr << "smash error: fg: invalid arguments" << "\n";
        return;
    }
    JobsList& jobsList = JobsList::getInstance();
    // the command valid
    if (argc == 1) {
        // fg
        if(jobsList.isEmpty()) {
            std::cerr << "smash error: fg: jobs list is empty" << "\n";
            return;
        }
        // jobs list is not empty
    } else {
        // fg <job-id>
        int jobId = _strToInt(argv[1]);
        if (!jobsList.isIn(jobId)) {
            std::cerr << "smash error: fg: job-id " << jobId << " does not exist" << "\n";
        }
        // the jobId exists in jobs list
    }
}

void BackgroundCommand::execute() {
    if (argc > 2 || argv[1] && !_isInteger(argv[1])) {
        std::cerr << "smash error: Bg: invalid arguments" << "\n";
        return;
    }
    JobsList& jobsList = JobsList::getInstance();
    if (argc == 1) {
        // bg
        // If bg was typed with no arguments (without job-id) but jobs list does not contain any
        // stopped job to resume in the background then the following error message should be
        // reported:
        // smash error: bg: there is no stopped jobs to resume
    } else {
        // bg <job-id>
        int jobId = _strToInt(argv[1]);
        if (!jobsList.isIn(_strToInt(argv[1]))) {
            std::cerr << "smash error: bg job-id " << jobId << " does not exist" << "\n";
        }
    }
    // If job-id exists but it is for a job which is already running in the background (not stopped)
    // then the following error message should be reported:
    // smash error: bg: job-id <job-id> is already running in the background
}

bool _isKill(char **args, int num) {
    for (int i = 0 ; i < num ; i++) {
        if(strcmp(args[i], "kill") == 0) {
            return true;
        }
    }
    return false;
}

void QuitCommand::execute() {
    bool isKill = false;
    if (_isKill(argv, argc)) {
        // cmd is quit kill
        JobsList& jobsList = JobsList::getInstance();
        int jobsCount = jobsList.getJobsCount();
        std::cout << "smash: sending SIGKILL signal to " << jobsCount << " jobs:" << "\n";
        jobsList.killAllJobs();
    } else  {
        // cmd is quit
        exit(0);
    }
}

void JobsCommand::execute() {
    JobsList& jobsList = JobsList::getInstance();
    jobsList.printJobsList();
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

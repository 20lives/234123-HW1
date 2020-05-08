#include <iostream>
#include <signal.h>

#include "signals.h"
#include "FgJob.h"

using namespace std;

void ctrlCHandler(int sig_num) {
    // TODO: Add your implementation
    // send SIGKILL to the process in the foreground
    std::cout << "smash: got ctrl-C" << "\n";
    FgJob& fgJob = FgJob::getInstance();
    if (fgJob.isFgJobRunning()) {
        pid_t pid = fgJob.getPid();
        if(kill(pid, SIGKILL) == 0) {
            std::cout << "msg: \"smash: process " << pid <<" was stopped" << "\n";
        }
    }
}

void ctrlZHandler(int sig_num) {
	// TODO: Add your implementation
	// send SIGTSTP to the process in the foreground: add the foreground process to the jobslist
//	std::cout << "smash: got ctrl-Z" << "\n";
//	FgJob& fgJob = FgJob::getInstance();
//	if (fgJob.isFgJobRunning()) {
//        pid_t pid = fgJob.getPid();
//        // msg: "smash: process <foreground-PID> was stopped"
//    }
}

void alarmHandler(int sig_num) {
  // TODO: Add your implementation
    std::cout << "smash got an alarm”" << "\n";
//    FgJob& fgJob = FgJob::getInstance();
//    if (fgJob.isFgJobRunning()) {
//        pid_t pid = fgJob.getPid();
//        // msg: "smash: process <foreground-PID> was stopped"
//    }
}


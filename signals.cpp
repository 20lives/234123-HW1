#include <iostream>
#include <signal.h>

#include "signals.h"
#include "Commands.h"

using namespace std;

void ctrlZHandler(int sig_num) {
	// TODO: Add your implementation
	// send SIGTSTP to the process in the foreground: add the foreground process to the jobslist
	std::cout << "smash: got ctrl-Z" << "\n";
    //pid_t pid =
	// msg: "smash: process <foreground-PID> was stopped"
}

void ctrlCHandler(int sig_num) {
  // TODO: Add your implementation
    // send SIGKILL to the process in the foreground
    std::cout << "smash: got ctrl-C" << "\n";
    // msg: "smash: process <foreground-PID> was killed"
}

void alarmHandler(int sig_num) {
  // TODO: Add your implementation
    std::cout << "smash got an alarm”" << "\n";
}


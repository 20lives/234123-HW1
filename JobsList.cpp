//
// Created by nomi on 5/6/20.
//

#include <vector>
#include <time.h>
#include <iostream>
#include <wait.h>
#include <unistd.h>


#include "JobsList.h"
#include "ExternalCommand.h"

/**
 * Aux functions
 */
/**
* Constructors and destructors
*/
JobsList::JobsList() {
    jobsList = {};
}

JobsList::JobEntry::JobEntry(string _command, bool _isStopped, int _jobId, pid_t _pid) : command(_command), isStopped(_isStopped), jobId(_jobId), jobPid(_pid) {
    startTime = time(0);
}

JobsList::~JobsList() {

}

/**
* getters and setters
*/
int JobsList::JobEntry::getJobId() {
    return jobId;
}

pid_t JobsList::JobEntry::getPid() {
    return jobPid;
}

time_t JobsList::JobEntry::getElapsed() {
    return (time(0) - startTime);
}

string JobsList::JobEntry::getCommandLine() {
    return command;
}

bool JobsList::JobEntry::getIsStopped() {
    return isStopped;
}

void JobsList::JobEntry::setContinue() {
  isStopped = false;
}

//void JobsList::JobEntry::setJobId(int _jobId) {
//    jobId = _jobId;
//}
/**
 * other class methods
 */
void JobsList::addJob(string command, pid_t pid, bool isStopped) {
    int jobId = getNextJobID(); // first jobId should be 1 !!
    JobEntry* entry = new JobEntry(command, isStopped, jobId, pid);
    jobsList.push_back(entry);
}

int JobsList::getNextJobID() {
    int maxId = 0;
    for(const auto& entry : jobsList) {
        maxId = entry->getJobId() > maxId ? entry->getJobId() : maxId;
    }
    return maxId + 1;
}

pid_t JobsList::getJobPid(int jobId) {
    for(const auto& entry : jobsList) {
        if (entry->getJobId() == jobId) {
            return entry->getPid();
        }
    }
    // return 0 ?????
    return -1;
}

bool JobsList::isEmpty() {
    return jobsList.empty();
}

bool JobsList::isIn(int jobId) {
    for(const auto& entry : jobsList) {
        if (entry->getJobId() == jobId) {
            return true;
        }
    }
    return false;
}

void JobsList::printJobsList() {
    for(const auto& entry : jobsList) {
        std::cout <<  "[" << entry->getJobId() << "] "
        << entry->getCommandLine() << " : " << entry->getPid() << " " << entry->getElapsed() << "secs";
        if (entry->getIsStopped() )
        {
            std::cout << " (stopped)";
        }
        std::cout << "\n";
        // [<job-id>] <command> : <process id> <seconds elapsed>
    }
}

JobsList::JobEntry *JobsList::getJobById(int jobId) {
    for(const auto& entry : jobsList) {
        if (entry->getJobId() == jobId) {
            return entry;
        }
    }
    return nullptr;
}

int JobsList::getJobsCount() {
    return jobsList.size();
}

void JobsList::killAllJobs() {
    for(const auto& entry : jobsList) {
        kill(entry->getPid(), SIGKILL);
        std::cout << entry->getPid()<< " : " << entry->getCommandLine() << "\n";
    }
}

void JobsList::removeFinishedJobs() {
    _List_iterator<JobEntry *> i = jobsList.begin();
    while (i != jobsList.end()) {
        pid_t pid = (*i)->getPid();
        pid_t status;
        string command = (*i)->getCommandLine();
        status = waitpid(pid, NULL, WNOHANG) ;

        if (status == -1) {
            jobsList.erase(i++);
            i++;
        }
        else {
            i++;
        }
    }
}

string JobsList::getJobCommnad(int jobId) {
    return getJobById(jobId)->getCommandLine();
}

int JobsList::getLastJob() {
    if (isEmpty()) return -1;
    return jobsList.back()->getJobId();
}

int JobsList::getLastStoppedJob() {
    for (auto i = jobsList.crbegin(); i != jobsList.crend(); ++i) {
        if((*i)->getIsStopped()) {
            return (*i)->getJobId();
        }
    }
    return -1;
}

bool JobsList::getIsStopped(int jobId) {
    return getJobById(jobId)->getIsStopped();
}

void JobsList::setContinue(int jobId) {
    getJobById(jobId)->setContinue();
}

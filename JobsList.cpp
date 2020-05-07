//
// Created by nomi on 5/6/20.
//

#include <vector>

#include "JobsList.h"

/**
 * Aux functions
 */
/**
* Constructors and destructors
*/
JobsList::JobsList() {
    jobsList = {};
}

JobsList::JobEntry::JobEntry(Command *_cmd, bool _isStopped, int _jobId) : cmd(_cmd), isStopped(_isStopped), jobId(_jobId) {
    //std::time(&time);
}

JobsList::~JobsList() {

}

/**
* getters and setters
*/
int JobsList::JobEntry::getJobId() {
    return jobId;
}

void JobsList::JobEntry::setJobId(int _jobId) {
    jobId = _jobId;
}
/**
 * other class methods
 */
void JobsList::addJob(Command* cmd, bool isStopped) {
    int jobId = getNextJobID();
    JobEntry* entry = new JobEntry(cmd, isStopped, jobId);
    jobsList.push_front(entry);
}

int JobsList::getNextJobID() {
    int maxId = -1;
    for(const auto& entry : jobsList) {
        maxId = entry->getJobId() > maxId ? entry->getJobId() : maxId;
    }
    return maxId + 1;
}

pid_t JobsList::getJobPid(int jobId) {
    // find pid with given jobId
    return 0;
}
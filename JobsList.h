//
// Created by nomi on 5/6/20.
//

#ifndef HW1_234123_JOBSLIST_H
#define HW1_234123_JOBSLIST_H

#include <string>
#include <time.h>
#include <list>
#include <vector>
#include "signals.h"
#include "Commands.h"

class JobsList {
private:
    class JobEntry {
    private:
        Command* cmd;
        bool isStopped;
        int jobId;
        time_t time;
        pid_t jobPid;
    public:
        JobEntry(Command* _cmd, bool _isStopped, int _jobId);
        int getJobId();
        void setJobId(int _jobId);
    };
    std::list<JobEntry*> jobsList;
    JobsList();
public:
    JobsList(JobsList const&) = delete;
    void operator=(JobsList const&) = delete;
    static JobsList& getInstance() {
        static JobsList instance;
        return instance;
    }
    ~JobsList();
    void addJob(Command* cmd, bool isStopped = false);
    void printJobsList();
    void killAllJobs();
    void removeFinishedJobs();
    JobEntry * getJobById(int jobId);
    void removeJobById(int jobId);
    JobEntry * getLastJob(int* lastJobId);
    JobEntry *getLastStoppedJob(int *jobId);
    pid_t getJobPid(int jobId);
    bool isEmpty();
    bool isIn(int jobId);
    // TODO: Add extra methods or modify existing ones as needed
    int getNextJobID();
};

#endif //HW1_234123_JOBSLIST_H

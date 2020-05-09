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
#include "ExternalCommand.h"

class JobsList {
private:
    class JobEntry {
    private:
        string command;
        bool isStopped;
        int jobId;
        time_t startTime;
        pid_t jobPid;
    public:
        JobEntry(string _command, bool _isStopped, int _jobId, pid_t _pid);
        int getJobId();
        pid_t getPid();
        time_t getElapsed();
        string getCommandLine();
        bool getIsStopped();
        void setContinue();
        void resetElapsed();
        void setIsStopped(bool _isStopped);
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
    void addJob(string command, pid_t pid, bool isStopped = false);
    void printJobsList();
    void killAllJobs();
    void removeFinishedJobs();
    JobEntry * getJobById(int jobId);
    int getJobByPid(pid_t pid);
    void removeJobById(int jobId);
    int getLastJob();
    int getLastStoppedJob();
    pid_t getJobPid(int jobId);
    bool getIsStopped(int jobId);
    void setContinue(int jobId);
    string getJobCommnad(int jobId);
    bool isEmpty();
    bool isIn(int jobId);
    int getNextJobID();
    int getJobsCount();
};

#endif //HW1_234123_JOBSLIST_H

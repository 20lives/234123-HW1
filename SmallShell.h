//
// Created by nomi on 5/6/20.
//

#ifndef HW1_234123_SMALLSHELL_H
#define HW1_234123_SMALLSHELL_H

#include "Commands.h"
#include "FgJob.h"

class Command;
class SmallShell {
private:
    std::string prevDir = "";
    std::string prompt = "smash";
    FgJob *currFgJob = NULL; // current foreground job - send signals to it if needed
    SmallShell();
public:
    bool isPrevDirSet = false;
    Command *CreateCommand(const char* cmd_line);
    SmallShell(SmallShell const&)      = delete; // disable copy ctor
    void operator=(SmallShell const&)  = delete; // disable = operator
    static SmallShell& getInstance() {
        static SmallShell instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }
    void executeCommand(const char* cmd_line);
    std::string getPrompt();
    void setPrompt(const char* _prompt);
    std::string getPrevDir();
    void setPrevDir(const char* _dir);
    ~SmallShell();
    // TODO: add extra methods as needed
};

#endif //HW1_234123_SMALLSHELL_H

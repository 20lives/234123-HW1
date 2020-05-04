#include <unistd.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <sys/wait.h>
#include <iomanip>
#include "Commands.h"
#include <linux/limits.h>

using namespace std;

const std::string WHITESPACE = " \n\r\t\f\v";

#if 0
#define FUNC_ENTRY()  \
    cerr << __PRETTY_FUNCTION__ << " --> " << endl;

#define FUNC_EXIT()  \
    cerr << __PRETTY_FUNCTION__ << " <-- " << endl;
#else
#define FUNC_ENTRY()
#define FUNC_EXIT()
#endif

#define DEBUG_PRINT cerr << "DEBUG: "

#define EXEC(path, arg) \
    execvp((path), (arg));

// should be in cdCommand class maybe?
enum CD_TYPE {
    ePrevDir, eChangeDir
};

// converts character array
// to string and returns it
string _convertToString(char* a, int size) {
    int i;
    string s = "";
    for (i = 0; i < size; i++) {
        s = s + a[i];
    }
    return s;
}

string _ltrim(const std::string& s) {
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}

string _rtrim(const std::string& s) {
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

string _trim(const std::string& s) {
    return _rtrim(_ltrim(s));
}

int _parseCommandLine(const char* cmd_line, char** args) {
    FUNC_ENTRY()
    int i = 0;
    std::istringstream iss(_trim(string(cmd_line)).c_str());
    for(std::string s; iss >> s; ) {
        args[i] = (char*)malloc(s.length()+1);
        memset(args[i], 0, s.length()+1);
        strcpy(args[i], s.c_str());
        args[++i] = NULL;
    }
    return i;
    FUNC_EXIT()
}

bool _isBackgroundComamnd(const char* cmd_line) {
    const string str(cmd_line);
    return str[str.find_last_not_of(WHITESPACE)] == '&';
}

void _removeBackgroundSign(char* cmd_line) {
    const string str(cmd_line);
    // find last character other than spaces
    unsigned int idx = str.find_last_not_of(WHITESPACE);
    // if all characters are spaces then return
    if (idx == string::npos) {
        return;
    }
    // if the command line does not end with & then return
    if (cmd_line[idx] != '&') {
        return;
    }
    // replace the & (background sign) with space and then remove all tailing spaces.
    cmd_line[idx] = ' ';
    // truncate the command line string up to the last non-space character
    cmd_line[str.find_last_not_of(WHITESPACE, idx) + 1] = 0;
}

SmallShell::SmallShell() {
// TODO: add your implementation
}

SmallShell::~SmallShell() {
// TODO: add your implementation
}

string SmallShell::getPrompt() {
    return prompt;
}

/**
* Creates and returns a pointer to Command class which matches the given command line (cmd_line)
*/
Command *SmallShell::CreateCommand(const char* cmd_line) {
    string cmd_s = string(cmd_line);
    if (cmd_s.find("chprompt") == 0) {
        return new ChangePromptCommand(cmd_line);
    } else if (cmd_s.find("showpid") == 0) {
        return new ShowPidCommand(cmd_line);
    } else if (cmd_s.find("pwd") == 0) {
        return new PwdCommand(cmd_line);
    } else if (cmd_s.find("cd") == 0) {
        return new CdCommand(cmd_line);
    }
    return nullptr;
}

Command::Command(const char* cmd_line) {
    argv = _parseCommandLine(cmd_line, argc);
}

char* Command::getArg(int i) {
    return argc[i];
}

int Command::getArgCount() {
    return argv;
}

BuiltInCommand::BuiltInCommand(const char* cmd_line) : Command(cmd_line) {

}

BuiltInCommand::~BuiltInCommand() {

}

ChangePromptCommand::ChangePromptCommand(const char* cmd_line) : BuiltInCommand(cmd_line) {

}

ShowPidCommand::ShowPidCommand(const char* cmd_line) : BuiltInCommand(cmd_line) {

}

PwdCommand::PwdCommand(const char* cmd_line) : BuiltInCommand(cmd_line) {

}

CdCommand::CdCommand(const char* cmd_line) : BuiltInCommand(cmd_line) {

}

void ChangePromptCommand::execute() {
    SmallShell& smash = SmallShell::getInstance();
    if (getArgCount() > 1) {
        smash.setPrompt(getArg(1));
    } else {
        smash.setPrompt("smash");
    }
}

void ShowPidCommand::execute() {
    std::cout << "smash pid is " << getpid() << "\n";
}

void PwdCommand::execute() {
    char currWorkingDir[PATH_MAX];
    getcwd(currWorkingDir, PATH_MAX);
    std::cout << currWorkingDir << "\n";
}

void  _changeDirectory(CD_TYPE cdType, char* dir = nullptr) {
    SmallShell& smash = SmallShell::getInstance();
    char currWorkingDir[PATH_MAX] = {};
    getcwd(currWorkingDir, PATH_MAX);
    string sCurrentWorkingDir = _convertToString(currWorkingDir, PATH_MAX);
    if (cdType == ePrevDir) {
        if (!smash.isPrevDirSet) {
            std::cout << "smash error: cd: OLDPWD not set" << "\n";
            return;
        }
        chdir(smash.getPrevDir().c_str());
    } else {
        chdir(dir);
    }
    smash.setPrevDir(currWorkingDir);
    smash.isPrevDirSet = true;
}

void CdCommand::execute() {
    if (getArgCount() > 2) {
        std::cout << "smash error: cd: too many arguments" << "\n";
    } else if (getArgCount() == 2) {
        if (strcmp(getArg(1), "-") == 0) {
            _changeDirectory(ePrevDir);
        } else {
            _changeDirectory(eChangeDir, getArg(1));
        }
    }
}

/**
 * SmallShell class implementations
 */
void SmallShell::executeCommand(const char *cmd_line) {
    // TODO: Add your implementation here

    // for example:
    Command* cmd = CreateCommand(cmd_line);
    if (cmd) {
        cmd->execute();
    }
// Please note that you must fork smash process for some commands (e.g., external commands....)
};

void SmallShell::setPrompt(const char *_prompt) {
    prompt = _prompt;
}

void SmallShell::setPrevDir(const char* _dir) {
    prevDir = _dir;
}

string SmallShell::getPrevDir() {
    return prevDir;
}
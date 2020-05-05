#include <unistd.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <sys/wait.h>
#include <iomanip>
#include "Commands.h"
#include <errno.h>

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

//enum SYS_CALL_TYPE {
//    eGetPid, eGetCwd, eChdir
//};

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

void _freeFields(char ** argv, int argc)
{
    for(int i = 0; i < argc; i++)
        free(argv[i]);
    //free(argv);
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
    cmd_line[str.find_last_not_of(WHITESPACE, idx)] = 0;
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

enum CMD_TYPE {
    eChprompt, eShowpid, ePwd, eCd, eExternal
};

void _setCmdType(char *cmd, CMD_TYPE *eCmdType) {
    if (strcmp(cmd, "chprompt") == 0) {
        *eCmdType = eChprompt;
    } else if (strcmp(cmd, "showpid") == 0) {
        *eCmdType = eShowpid;
    } else if (strcmp(cmd, "pwd") == 0) {
        *eCmdType = ePwd;
    } else if (strcmp(cmd, "cd") == 0) {
        *eCmdType = eCd;
    } else {
        *eCmdType = eExternal;
    }
}

/**
* Creates and returns a pointer to Command class which matches the given command line (cmd_line)
*/
Command *SmallShell::CreateCommand(const char* cmd_line) {
    char *argv[COMMAND_MAX_ARGS] = {};
    int argc = _parseCommandLine(cmd_line, argv);

    if (argc == 0) {
        _freeFields(argv, argc);
        return nullptr;
    }
    char cmdLine[COMMAND_ARGS_MAX_LENGTH];
    strcpy(cmdLine, cmd_line);
    bool isBackgroundCmd;
    CMD_TYPE eCmdType;

    // 1. check if &
    isBackgroundCmd = (string(cmd_line).back() == '&');
    // 2. remove last &
    _removeBackgroundSign(cmdLine);
    // now we have the cmd line with out the last & and we have a boolean value indicating if this is a background cmd
    // 3. set cmd type
    _setCmdType(argv[0], &eCmdType);
    // 4. free the cmdLine
    _freeFields(argv, argc);
    // 5. create te command
    if (eCmdType == eChprompt) {
        return new ChangePromptCommand(cmdLine);
    } else if (eCmdType == eShowpid) {
        return new ShowPidCommand(cmdLine);
    } else if (eCmdType == ePwd) {
        return new PwdCommand(cmdLine);
    } else if (eCmdType == eCd) {
        return new CdCommand(cmdLine);
    } else {
        // external cmd is the last possibly
        return new ExternalCommand(cmdLine, isBackgroundCmd);
    }
    return nullptr;
}

Command::Command(const char* cmd_line) {
    //argc = _parseCommandLine(cmd_line, argv);
}

char* BuiltInCommand::getArg(int i) {
    return argv[i];
}

int BuiltInCommand::getArgCount() {
    return argc;
}

Command::~Command() {

}

BuiltInCommand::BuiltInCommand(const char* cmd_line) : Command(cmd_line) {
    argc = _parseCommandLine(cmd_line, argv);
}

BuiltInCommand::~BuiltInCommand() {
    _freeFields(argv, argc);
}

ChangePromptCommand::ChangePromptCommand(const char* cmd_line) : BuiltInCommand(cmd_line) {

}

ShowPidCommand::ShowPidCommand(const char* cmd_line) : BuiltInCommand(cmd_line) {

}

PwdCommand::PwdCommand(const char* cmd_line) : BuiltInCommand(cmd_line) {

}

CdCommand::CdCommand(const char* cmd_line) : BuiltInCommand(cmd_line) {

}

ExternalCommand::ExternalCommand(const char* cmd_line, bool _isBackgroundCmd) : isBackgroundCmd(_isBackgroundCmd), Command(cmd_line) {
    strcpy(cmdLine, cmd_line);
    cmdLineLength = string(cmd_line).length();
}

void ExternalCommand::execute() {
    // update args array
    char *args[4] = {};
    args[0] = (char*)malloc(string("/bin/bash").length() + 1);
    strcpy(args[0], "/bin/bash");
    args[1] = (char*)malloc(string("-c").length() + 1);
    strcpy(args[1], "-c");
    args[2] = (char*)malloc(cmdLineLength + 1);
    strcpy(args[2], (string(cmdLine)).c_str());
    args[3] = NULL;

    pid_t pid = fork();
    if (pid == 0) {
        execv("/bin/bash", args);
    } else {
        // parent pid
        if (!isBackgroundCmd){
            // in case this is not a background cmd we will wait for the child process to finish
            wait(NULL);
        }
    }
}

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

void  _changeDirectory(CD_TYPE cdType, char* dir = nullptr) {
    SmallShell& smash = SmallShell::getInstance();
    char currWorkingDir[COMMAND_ARGS_MAX_LENGTH] = {};
    char newWorkingDir[COMMAND_ARGS_MAX_LENGTH] = {};
    getcwd(currWorkingDir, COMMAND_ARGS_MAX_LENGTH);
    if (cdType == ePrevDir) {
        if (!smash.isPrevDirSet) {
            std::cout << "smash error: cd: OLDPWD not set" << "\n";
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
    delete(cmd);
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
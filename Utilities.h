//
// Created by nomi on 5/6/20.
//

#ifndef HW1_234123_UTILITIES_H
#define HW1_234123_UTILITIES_H

#include <string>

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

#define COMMAND_ARGS_MAX_LENGTH (200)
#define COMMAND_MAX_ARGS (20)
#define HISTORY_MAX_RECORDS (50)

using namespace std;

const std::string WHITESPACE = " \n\r\t\f\v";

enum CMD_TYPE {
    eChprompt, eShowpid, ePwd, eCd, eKill, eFg, eExternal
};

string _ltrim(const std::string& s);

string _rtrim(const std::string& s);

string _trim(const std::string& s);

int _parseCommandLine(const char* cmd_line, char** args);

void _freeFields(char ** argv, int argc);

bool _isBackgroundComamnd(const char* cmd_line);

void _removeBackgroundSign(char* cmd_line);

#endif //HW1_234123_UTILITIES_H

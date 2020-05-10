//
// Created by nomi on 5/9/20.
//

#include <string.h>
#include <iostream>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>

#include "Utilities.h"
#include "PipeCommand.h"



PipeCommand::PipeCommand(const char *cmd_line, bool _isBackground, bool _isErr) : Command(cmd_line) {
    strcpy(rawCmdLine, cmd_line);
    isBackground = _isBackground;
    isErr = _isErr;

    char cmdLine[COMMAND_ARGS_MAX_LENGTH] = {};
    strcpy(cmdLine, cmd_line);
    _removeBackgroundSign(cmdLine);

    std::size_t found = string(cmd_line).find_first_of('|');
    if (!_isErr) {
        strcpy(firstCmd, string(cmdLine).substr(0, found).c_str());
        strcpy(secondCmd, string(cmdLine).substr(found + 1).c_str());
    } else {
        strcpy(firstCmd, string(cmdLine).substr(0, found - 1).c_str());
        strcpy(secondCmd, string(cmdLine).substr(found + 1).c_str());
    }
    //strcpy(file, string(file).substr(0, string(file).find(" ")).c_str());
    //std::cout << "cmd: " << firstCmd << "file: " << secondCmd << '\n';

}

 void _transfer() {// transfer chars from STDIN to STDOUT
    char c;
    size_t ret = 1;
    while ((read(0, &c, 1) > 0) && ret > 0)
    ret = write(1, &c, 1);
    exit(0);
}
void PipeCommand::execute() {
    SmallShell& smallShell = SmallShell::getInstance();
    PipeInfo& pipeInfo = PipeInfo::getInstance();
    pipeInfo.isPipe = true;
    pipeInfo.isBackground = this->isBackground;
    pipeInfo.isErr = this->isErr;
    strcpy(pipeInfo.rawCmdLine, rawCmdLine);
    pipeInfo.pid = getpid();

    int origin_out = dup(1);
    pipeInfo.origin_out = origin_out;
    int origin_err = dup(2);
    pipeInfo.origin_err = origin_err;

    int fd[2];

    int fdToChange = (!isErr) ? 1 : 2;
    SmallShell& smash = SmallShell::getInstance();

    pid_t pid = fork();
    setpgrp();
    if (pid == 0) {
        // son
        pipe(fd);
        if (fork() == 0) {
            // son 1
            //std::cout << "son 1" << '\n';
            dup2(fd[1], fdToChange);

            close(fd[0]);
            close(fd[1]);

            smash.executeCommand(firstCmd);
            exit(0);
        }
        if (fork() == 0) {
            // son 2
            //std::cout << "son 2" << '\n';
            dup2(fd[0], 0);

            close(fd[0]);
            close(fd[1]);

            smash.executeCommand(secondCmd);
            exit(0);
        }

        close(fd[0]);
        close(fd[1]);

        close(1);
        //dup(pipeInfo.origin_out);
        close(2);
        //dup(pipeInfo.origin_err);

        exit(0);

    } else {
        // parent
        //std::cout << "parent" << '\n';

    }


//    int fd_out = (!isErr) ? open(file,  O_WRONLY|O_CREAT|O_TRUNC,0666) : open(file, O_WRONLY|O_CREAT|O_TRUNC | O_APPEND,0666);
//    close(1);
//    dup(fd_out);
//    close(fd_out);

    //smallShell.executeCommand(cmd);

//    close(1);
//    dup(origin_out);
//    close(origin_out);
//    redInfo.isBackground = false;
//    redInfo.isRedirection = false;
}
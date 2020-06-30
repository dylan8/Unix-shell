#define _GNU_SOURCE
#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <limits.h>
#include <pwd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "cmds.h"
#include "execute.h"
#define MAXL 1000
#define MAXC 100

int input(char* str)
{
    char* buffer;

    buffer = readline("\n# ");
    if (strlen(buffer) != 0) {
        add_history(buffer);
        strcpy(str, buffer);
        return 0;
    } else {
        return 1;
    }
}

int cmdHandler(char** parsed)
{
        int NoOfOwnCmds = 6, i, switchOwnArg = 0;
        char *ListOfOwnCmds[NoOfOwnCmds];
        char cwd[PATH_MAX];
        register struct passwd *pw;
        register uid_t uid;
        uid = geteuid();
        pw = getpwuid(uid);

        ListOfOwnCmds[0] = "exit";
        ListOfOwnCmds[1] = "cd";
        ListOfOwnCmds[2] = "pwd";
        ListOfOwnCmds[3] = "whoami";
        ListOfOwnCmds[4] = "help";
        ListOfOwnCmds[5] = "echo";

        for (i = 0; i < NoOfOwnCmds; i++) {
            if (strcmp(parsed[0], ListOfOwnCmds[i]) == 0) {
                switchOwnArg = i + 1;
                break;
            }
        }

        switch (switchOwnArg) {
            case 1:
                printf("\nGoodbye\n");
                exit(0);
            case 2:
                chdir(parsed[1]);
                return 1;
            case 3:
                if (getcwd(cwd, sizeof(cwd)) != NULL) {
                    printf("Current working dir: %s\n", cwd);
                } else {
                    perror("getcwd() error");
                    return 1;
                }
                return 1;
            case 4:
                if (pw) {
                    puts(pw->pw_name);
                    return 1;
                }
            case 5:
                help();
                return 1;
            case 6:
                cmdEcho(parsed);
                break;
            default:
                break;
        }
    return 0;
}

int parsePipe(char* string, char** stringpiped)
{
    int i;
    for (i = 0; i < 2; i++) {
        stringpiped[i] = strsep(&string, "|");
        if (stringpiped[i] == NULL)
            break;
    }

    if (stringpiped[1] == NULL)
        return 0; // if no pipe is found it returns 0
    else {
        return 1;
    }
}

//parse command words
void parseSpace(char* str, char** parsed)
{
    int i;
    for (i = 0; i < MAXC; i++) {
        parsed[i] = strsep(&str, " ");
        if (parsed[i] == NULL)
            break;
        if (strlen(parsed[i]) == 0)
            i--;
    }
}

int processString(char* str, char** parsed, char** parsedpipe)
{
    char* strpiped[2];
    int piped = 0;
    piped = parsePipe(str, strpiped);

    if (piped) {
        parseSpace(strpiped[0], parsed);
        parseSpace(strpiped[1], parsedpipe);
    } else {
        parseSpace(str, parsed);
    }
    if (cmdHandler(parsed))
        return 0;
    else
        return piped+1;
}

int main(void)
{
    char inputString[MAXL], *parsedArgs[MAXC];
    char* parsedArgsPiped[MAXC];
    int execFlag = 0;

    struct tm* local;
    time_t t = time(NULL);
    local = localtime(&t);

    printf("Local time and date: %s\n",
           asctime(local));

    if (signal(SIGINT, sig_handler) == SIG_ERR)
        printf("\nunable to catch SIGINT\n");
    while (1) {
        // input
        if (input(inputString))
            continue;
        // process
        execFlag = processString(inputString,
                                 parsedArgs, parsedArgsPiped);
        // execflag returns zero if there is no command
        // or it is a builtin command,
        // 1 if it is a simple command
        // 2 if it is including a pipe.

        // execute
        if (execFlag == 1)
            execArgs(parsedArgs);

        if (execFlag == 2)
            execArgsPiped(parsedArgs, parsedArgsPiped);
    }
    return 0;


}
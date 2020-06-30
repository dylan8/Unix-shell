//
// Created by kearn on 19/03/2020.
//
#include "stdio.h"
#include <signal.h>
#include <limits.h>
#include "cmds.h"
void cmdEcho(char **parsed){
    int i = 1;
    while(parsed[i] != NULL){
        printf("%s ", parsed[i]);
        i++;
    }
    printf("\n");
}

char *getcwd(char *buf, size_t size);

void sig_handler(int SIG)
{
    if (SIG == SIGINT)
        printf("caught SIGINT\n");
}

void help() {
    printf("************************************************\n");
    printf("*\tWelcome to Dylan's terminal.*\n");
    printf("************************************************\n");
    printf("Your options of commands are as follows: 'help' - will display help message\n'exit' - will end the command prompt\n'ls' - will display the current directory of folders\n'cd' - will allow you to change your current working directory.\n'whoami' - will show you what user is logged on.\n'pwd - will show you the current working directory ");
    printf("\n");
}


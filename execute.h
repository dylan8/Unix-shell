//
// Created by kearn on 19/03/2020.
//
#include<stdio.h>
#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
#include <string.h>
#include<unistd.h>
#include<unistd.h>
#include<sys/types.h>


void execArgs(char** parsed)
{
    // Forking a child
    pid_t pid = fork();
    if (pid == -1) {
        printf("\nUnable to fork child");
        return;
    } else if (pid == 0) {
        if (execvp(parsed[0], parsed) < 0) {
            printf("\nCouldn't execute command");
        }
        exit(0);
    } else {
        wait(NULL);
        return;
    }
}

void execArgsPiped(char** parsed, char** parsedpipe)
{
    int pipefd[2];
    pid_t p1, p2;

    if (pipe(pipefd) < 0) {
        printf("\nPipe couldn't be initialized");
        return;
    }
    p1 = fork();
    if (p1 < 0) {
        printf("\nCouldn't fork");
        return;
    }

    if (p1 == 0) {
        // Child 1 executes
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        if (execvp(parsed[0], parsed) < 0) {
            printf("\nCould not execute command 1..");
            exit(0);
        }
    } else {
        // Parent executing
        p2 = fork();

        if (p2 < 0) {
            printf("\nCould not fork");
            return;
        }

        // Child 2 executing..
        // It only needs to read at the read end
        if (p2 == 0) {
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            if (execvp(parsedpipe[0], parsedpipe) < 0) {
                printf("\nCould not execute command 2..");
                exit(0);
            }
        } else {
            close(pipefd[1]);
            wait(NULL);
            wait(NULL);
        }
    }
}
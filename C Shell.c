
//
//  main.c
//  0516213
//
//  Created by 王嵩允 on 2018/10/3.
//  Copyright © 2018年 王嵩允. All rights reserved.
//
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_LINE 80
#define Delim " \n"

char *arg[MAX_LINE/2 + 1]; /* command line arguments */
int should_run = 1; /* flag to determine when to exit program */

char buffer[100];
int command_count, BackOrNot = 0;
pid_t pid;

void initialize(){
    int i = 100;
    while(i--){
        buffer[i] = '\0';
    }
    
    command_count = 0;
}

void readline(){
    read(STDIN_FILENO, buffer, MAX_LINE);
    
    arg[command_count] = strtok(buffer, Delim);
    
    while (arg[command_count] != NULL){
        //printf("%s\n", arg[command_count]);
        command_count++;
        
        arg[command_count] = strtok(NULL, Delim); // it will +1 after strtok
    }
}

void background(){
    if(strcmp(arg[command_count-1], "&") == 0){
        arg[command_count-1] = NULL; // out of '&'
        BackOrNot = 1;
    }
    else{
        BackOrNot = 0;
    }
    //printf("%d\n", BackOrNot);
}

void execute_cd(){
    if (chdir(arg[1]) != 0) {
        perror("Error");
    }
}

void execute_exit(){
    should_run = 0;
}

void execute_other(){
    pid = fork();
    
    if(pid < 0){ // Error Occur
        should_run = 0;
    }
    else if (pid == 0){
        if (execvp(arg[0], arg) != 0) {
            printf("It's not a command.\n");
            should_run = 0;
        }
        
    }
    else{
        if (BackOrNot == 0){
            waitpid(pid, NULL, 0);
            //printf("Child Complete!\n");
        }
    }
}

int main(int argc, char *argv[]){
    
    while(should_run){
        printf("osh>");
        fflush(stdout);
        
        initialize(); // initialize parameter
        readline(); // read command
        background(); // run in background or not
        
        // user input instruction jugdement
        if(strcmp(arg[0], "cd") == 0) // Instruction of "cd"
            execute_cd();
        
        else if(strcmp(arg[0], "exit") == 0) // Instruction of "exit"
            execute_exit();
        
        else
            execute_other();
        
    }
    
    return 0;
}

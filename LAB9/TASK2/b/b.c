#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

void sigchild_handler(int signum){
     pid_t pid;
     int status;
     
     while((pid = waitpid(-1, &status, WNOHANG)) > 0){
        printf("Child process with PID %d terminated\n", pid);
        if(WIFEXITED(status)){
          printf("Exit status: %d\n", WEXITSTATUS(status));
        }
        fflush(stdout);
     }
}

int main(){
    signal(SIGCHLD, sigchild_handler);
    
    pid_t pid = fork();
    if(pid == -1){
       perror("fork failed");
       exit(1);
    }else if(pid == 0){
        printf("Child (PID %d) is running...\n", getpid());
        fflush(stdout);
        sleep(2);
        exit(42);
    }else{
        printf("Parent (PID %d) waiting for child...\n", getpid());
        fflush(stdout);
        while(1){
             pause();
        }
    }
       
     return 0;
}

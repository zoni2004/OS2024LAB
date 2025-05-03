#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

void proc_exit(int signum){
     int wstat;
     pid_t pid;

     while(1){
        pid = waitpid(-1, &wstat, WNOHANG);
        if(pid == 0 || pid == -1){
            fprintf(stdout, "return value of waitpid() is %d\n", pid);
            return;
         }
         fprintf(stdout, "Return code: %d\n", WEXITSTATUS(wstat));
     }
}

int main(){
    signal(SIGCHLD, proc_exit);
    switch (fork()){
    case -1:
       perror("main: fork");
       exit(0);
    case 0:
       printf("I'm alive (temporarily)\n");
       int ret_code = rand();
       printf("Return code is %d\n", ret_code);
    default:
       pause();
    }
   exit(0);
}

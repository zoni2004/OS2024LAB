#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void sigint_handler(int signum){
      fprintf(stdout, "SIGINT received but ignored. Signal number: %d\n", signum);
}

int main(){
     if(signal(SIGINT, sigint_handler) == SIG_ERR){
         printf("Error setting up signal handler for SIGINT\n");
         return 1;
     }
    
     printf("Press Ctrl+C (SIGINT). It will be caught but ignored (not terminating)....\n");

     while(1){
       sleep(1);
    }

    return 0;
}

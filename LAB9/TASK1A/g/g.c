#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void sigint_handler(int signum){
      printf("\nCtrl+C (SIGINT) received. Teminating..\n");
      exit(EXIT_SUCCESS);
}

void my_handler(int signum){
      printf("\nSIGUSR1 received (signum: %d). Custom action triggered!\n", signum);
}

int main(){
     signal(SIGINT, sigint_handler);
     
     signal(SIGUSR1, my_handler);
     
     printf("Process PID: %d\n", getpid());
     printf("Options:\n");
     printf("1. Press Ctrl+C to terminate normally\n");
     printf("2. Run 'kill -SIGUSR1 %d' from another terminal to trigger custom handler.\n", getpid());
     printf("3. Waiting for signals....\n");
     
     sleep(3);
     printf("\nRaising SIGUSR1 internally...\n");
     raise(SIGUSR1);
      
      while(1){
         sleep(1);
      }

      return EXIT_SUCCESS;
}

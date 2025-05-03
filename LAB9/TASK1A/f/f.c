#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void sigint_handler(int signum){
      printf("\nCtrl+C (SIGINT) received.Force-killing the process..\n");
      fflush(stdout);
      raise(SIGKILL);
}

int main(){
      struct sigaction sa;
      sa.sa_handler = sigint_handler;
      sigemptyset(&sa.sa_mask);
      sa.sa_flags = 0;

      if(sigaction(SIGINT, &sa, NULL) == -1){
         perror("sigaction");
         return EXIT_FAILURE;
      }
      
      printf("Press Ctrl+C to kil this process.\n");
      printf("Process PID: %d\n", getpid());
      while(1){
         sleep(1);
      }

      return EXIT_SUCCESS;
}

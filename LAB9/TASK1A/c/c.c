#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void signal_handler(int signum){
      printf("Ctrl+C (SIGINT) received. Terminating..\n");
      fflush(stdout);
      exit(EXIT_SUCCESS);
}

int main(){
      struct sigaction sa;
      sa.sa_handler = signal_handler;
      sigemptyset(&sa.sa_mask);
      sa.sa_flags = 0;

      if(sigaction(SIGINT, &sa, NULL) == -1){
         perror("sigaction");
         return EXIT_FAILURE;
      }
      
      printf("Press CTrl+C to test. Running indefinitely...\n");
      while(1){
         sleep(1);
      }

      return EXIT_SUCCESS;
}

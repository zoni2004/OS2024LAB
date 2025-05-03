#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void signal_handler(int signum){
      printf("Ctrl+C (SIGINT) received. Exitinf..\n");
      exit(signum);
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

      while(1){
         sleep(1);
      }

      return EXIT_SUCCESS;
}

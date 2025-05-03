#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void sigint_handler(int signum){
     fprintf(stdout, "Caught SIGINT signal (%d). Terminating program.\n" , signum);
     exit(0);
}

int main(){
     if(signal(SIGINT, sigint_handler) == SIG_ERR){
         printf("Error setting up signal handler for SIGINT\n");
         return 1;
      }

      printf("Press Ctrl+C (SIGINT). Program will catch and then terminate...\n");

      while(1){
         sleep(1);
      }

      return 0;
}

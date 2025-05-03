#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main(){
      signal(SIGINT, SIG_IGN);
      
      printf("Process PID: %d\n", getpid());
      printf("Ctrl+C disabled. Use 'kill %d' from another terminal to terminate.\n", getpid());

      while(1){
         sleep(1);
      }

      return 0;
}

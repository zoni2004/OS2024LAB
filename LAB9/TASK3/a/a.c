#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

#define NUM_THREADS 4
pthread_t threads[NUM_THREADS];

void sigusr1_handler(int signum){
     pid_t pid = getpid();
     pthread_t tid = pthread_self();
     fprintf(stdout, "Thread %lu received SIGUSR1 signal (parent=%u) [pid=%u] (tid=%lu) \n:",(unsigned long)tid, getppid(), pid, (unsigned long)tid);
     fflush(stdout);
}

void *thread_function(void *arg){
     while(1) sleep(1);
     return NULL;
}

int main(){
    signal(SIGUSR1, sigusr1_handler);
    for(int i=0; i<NUM_THREADS; ++i){
        if(pthread_create(&threads[i], NULL, thread_function, NULL) != 0){
           perror("pthread_create"); 
           exit(EXIT_FAILURE);
        }
    }
    pid_t pid = getpid();
    pthread_t main_tid = pthread_self();
    fprintf(stdout, "Parent Process ID: %u, Process ID: %u, main() thread ID: %lu\n", getppid(), pid, (unsigned long)main_tid);
    fprintf(stdout, "Thread IDs: 0=%lu, 1=%lu, 2=%lu, 3=%lu\n", (unsigned long)threads[0], (unsigned long)threads[1], (unsigned long)threads[2], (unsigned long)threads[3]);
    fflush(stdout);
    
    kill(pid, SIGUSR1);
    pthread_kill(threads[2], SIGUSR1);

    for(int i=0; i<NUM_THREADS; ++i){
        if(pthread_join(threads[i], NULL) != 0){
           perror("pthread_join");
           exit(EXIT_FAILURE);
        }
    }
    return 0;
}

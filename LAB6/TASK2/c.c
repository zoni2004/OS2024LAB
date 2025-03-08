#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#define FIFO_FILE "/tmp/myfifo"

int main(){
    int fd;
    char buffer[BUFSIZ];
    ssize_t num_bytes;

    fd = open(FIFO_FILE, O_RDONLY);
    if(fd == -1){
       perror("open");
       exit(EXIT_FAILURE);
    }
    while(1){
       num_bytes = read(fd, buffer, BUFSIZ);
       if(num_bytes>0){
            buffer[num_bytes] = '\0';
            printf("Consumer: Received message: %s", buffer);
       }
       if(strncmp(buffer, "exit", 4) == 0){
           break;
       }
}

close(fd);
return 0;
}

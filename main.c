#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/wait.h>

#include "server.c"
//#include "ring_buffer.c"
//#include "ringbuffer.h"

int main(){
	pid_t pid = 0;
    int succes = 0;
    char str[512];

	for(int i=0; i<3; i++){
		pid = fork();
		if(pid == 0){ 
            //printf("child, pid = %d\n", getpid());
            sleep(1);
            break;
        }
		else if(pid > 0){ //parent
			//printf("parent, pid = %d, ppid = %d\n", getpid(), getppid());
		}
    }
    if(pid == 0){
        printf("child: pid = %d, ppid = %d\n", getpid(), getppid());
        connect_socket(getppid()); //連接parent的socket
    }else{
        printf("parent: pid = %d\n", getpid());
        succes = create_socket(getpid()); //以pid為port創建socket
        if(!succes) exit(1);
    }

    return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/wait.h>

#include "server.c"


int main(){
	pid_t pid = 0;
    int succes = 0;
    char str[512];
    
    //ring_buf_t rbuf;
    //ring_buf_init(&rbuf, 3, sizeof(str));

	for(int i=0; i<3; i++){
		pid = fork();
		if(pid == 0){ 
            //printf("child, pid = %d\n", getpid());
            break;
        }
		else if(pid > 0){ //parent
			//printf("parent, pid = %d, ppid = %d\n", getpid(), getppid());
		}
    }
    if(pid == 0){
        sleep(1);
        printf("child: pid = %d connect to 127.0.0.1:%d\n", getpid(), getppid());
        connect_socket(getppid()); //連接parent的socket
    }else{
        printf("parent: pid = %d\n", getpid());
        succes = create_socket(getpid()); //以pid為port創建socket
        if(!succes) exit(1);
    }

    return 0;
}

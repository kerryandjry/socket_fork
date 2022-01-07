#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>

#include "ring_buffer.h"
#include "ring_buffer.c"
#include "client.c"

int create_socket(int pid){
    char str[1024];
    int sockfd, clientfd;
    ring_buf_t rbuf;
    if(ring_buf_init(&rbuf, 3, sizeof(str))) printf("ring buffer init\n");
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        printf("Socket Error...\n");
        return -1;
    }

    memset(&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(pid);
    printf("Create socket on %s:%d\n", "127.0.0.1", pid);
    if(bind(sockfd, (struct sockaddr*) &server_addr, sizeof(server_addr)) == -1){
    printf("bind error...\n");
        return -1;
    }
    
    if(listen(sockfd, 10) == 0) printf("Listening...\n");
    else{
        printf("Binding Error!!!\n");
        return -1;
    }
    //for(int i=0; i<3; i++){
   while(1){
        clientfd = accept(sockfd, (struct sockaddr*) &client_addr, &addr_size);
        if(clientfd == -1){
            printf("Accept error...\n");
            break;
        }
       // printf("Connetec from child...\n");
        
        recv(clientfd, str, sizeof(str), 0);
        //if(str[0] == 'e' && str[1] == 'x' && str[2] == 'i' && str[3] == 't') break;
        if(str[0] == '$'){
            client_socket(str+1);
            //send(clientfd, str+6, strlen(str+6), 0);
            bzero(str, sizeof(str));
            //close(clientfd);
        }
        else{
            printf("Server get %s\n", str);
            ring_buf_write(&rbuf, str);
            send(clientfd, str, strlen(str), 0);
            bzero(str, sizeof(str));
        }

    }
    close(sockfd);
    
    if(!ring_buf_is_empty(&rbuf)){
        printf("Ring Buffer get:\n:");
        for(int i=0; i<rbuf.depth; i++){
            if(ring_buf_read(&rbuf, str)) 
                printf("%s\n", str);
        }
        ring_buf_delete(&rbuf);
        printf("\n");
    }else if(ring_buf_is_empty(&rbuf)) printf("Ring Buffer empty\n");

}

int connect_socket(int pid){
    int sock;
    char str[1024];
    struct sockaddr_in server_addr;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock<0){
        printf("Socket Error...\n");
        return -1;
    }
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(pid);
    
    if(connect(sock, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) printf("Connect error...\n");

    //bzero(str, sizeof(str));
    printf("client: \n");
    scanf("%s", &str[0]);
    send(sock, str, strlen(str), 0);
    if(str[0] != '$'){
        recv(sock, str, sizeof(str), 0);
        printf("recive from server: %s\n", str);
    }

    close(sock);
}

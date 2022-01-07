#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>

void client_socket(char* port_message){
    int sock;
    char* message[1024];
    for(int i=0; i<strlen(port_message); i++)
        if(port_message[i] == '$'){
            port_message[i] = '\0';
            *message = &port_message[i+1];
            break;
        }
    
    struct sockaddr_in server_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0){
         printf("Socket Error...\n");
         exit(1);
     }
    
     server_addr.sin_family = AF_INET;
     server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
     server_addr.sin_port = htons(atoi(port_message));

     if(connect(sock, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0){
         printf("Connect to other server error...\n");
         exit(1);
     }
     else printf("Connect to 127.0.0.1:%d\n", atoi(port_message));

     //bzero(str, sizeof(str));
     printf("client: \n");
     send(sock, *message, strlen(*message), 0);
     bzero(*message, sizeof(*message));
     if(recv(sock, *message, sizeof(*message), 0) <= 0){ 
         printf("receving error...\n");
     }else{
         printf("from other server: %s\n", *message);
     }

     close(sock);
}

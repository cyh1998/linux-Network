#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>

#define MAX_SIZE 1024

using namespace std;

int main(){
    int socket_fd;
    int recv_res;
    int send_res;
    char get_buf[MAX_SIZE];
    char send_buf[MAX_SIZE];
    int len;

    struct sockaddr_in client_addr;
    memset(&client_addr,0,sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(8888);
    client_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    len = sizeof(client_addr);


    socket_fd = socket(AF_INET,SOCK_DGRAM,0);
    if(socket_fd < 0){
         cout << "socket error!" << endl;
         exit(1);
    }

    //fun_res = bind(socket_fd,(struct sockaddr *)&client_addr,len)
    if(bind(socket_fd,(struct sockaddr *)&client_addr,len) < 0){
        cout << "bind error!" << endl;
        exit(1);
    }

    while(1){
        //cout << "server wait..." << endl;
        recv_res = recvfrom(socket_fd,&get_buf,sizeof(get_buf),0,(struct sockaddr *)&client_addr,(socklen_t *)&len);
        if(recv_res < 0){
            cout << "recvfrom error!" << endl;
            exit(1);
        }

        get_buf[recv_res] = 0;
        cout << "client msg: " << get_buf << endl;

        cin >> send_buf;

        send_res = sendto(socket_fd,send_buf,strlen(send_buf),0,(struct sockaddr *)&client_addr,len);
        if(send_res < 0){
            cout << "sendto error!" << endl;
            exit(1);
        }
    }

    close(socket_fd);

    return 0;
}

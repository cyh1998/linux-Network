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

    int socket_fd; //socket文件描述符
    int send_res;
    int get_res;
    int len;
    char send_buf[MAX_SIZE];
    char get_buf[MAX_SIZE];
    string str;

    struct sockaddr_in server_addr;
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8888);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    len = sizeof(server_addr);

    socket_fd = socket(AF_INET,SOCK_DGRAM,0);
    if(socket_fd < 0){
        cout << "socket error!" << endl;
        exit(1);
    }

    while(1){

        //cout << "Input your msg: ";
        cin >> str;

        if(str == "exit"){
            exit(1);
        }else{
            strcpy(send_buf,str.c_str());
        }

        send_res = sendto(socket_fd,send_buf,strlen(send_buf),0,(struct sockaddr *)&server_addr,len);
        if(send_res < 0){
            cout << "sendto error!" << endl;
            exit(1);
        }

        get_res = recvfrom(socket_fd,&get_buf,sizeof(get_buf),0,(struct sockaddr *)&server_addr,(socklen_t *)&len);
        if(get_res < 0){
            cout << "recvfrom error!" << endl;
            exit(1);
        }

        get_buf[get_res] = 0;
        cout << "server msg:" << get_buf << endl;

    }

    close(socket_fd);

    return 0;
}

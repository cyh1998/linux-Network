#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAXSIZE 2048

using namespace std;

int main(){

    int socket_fd;
    int conn_fd;

    int fun_res;
    int recv_num;
    char get_buff[MAXSIZE];
    char send_buff[MAXSIZE];
    struct sockaddr_in client_addr;
    string str;
    int len;

    fd_set rfds;
    struct timeval tv;
    int max_fd;

    memset(&client_addr,0,sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(8888);
    client_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    len = sizeof(client_addr);

    socket_fd = socket(AF_INET,SOCK_STREAM,0);
    if(socket_fd < 0){
        cout << "socket error!" << endl;
        exit(1);
    }

    fun_res = bind(socket_fd,(struct sockaddr *)&client_addr,len);
    if(fun_res < 0){
        cout << "bind error!" << endl;
        exit(1);
    }

    fun_res = listen(socket_fd,5);
    if(fun_res < 0){
        cout << "listen error!" << endl;
        printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
        exit(1);
    }

    conn_fd = accept(socket_fd,(struct sockaddr *)&client_addr,(socklen_t *)&len);
    if(conn_fd < 0){
        cout << "accept error!" << endl;
        exit(1);
    }

    while(1){
        //select()
        FD_ZERO(&rfds);
        FD_SET(0,&rfds);

        max_fd = 0;

        FD_SET(conn_fd,&rfds);

        if(max_fd < conn_fd) max_fd = conn_fd;

        tv.tv_sec = 5;
        tv.tv_usec = 0;

        fun_res = select(max_fd+1,&rfds,NULL,NULL,&tv);
        if(fun_res < 0){
            cout << "select error!" << endl;
            exit(1);
        }else if(fun_res == 0){
            //cout << "no msg!waiting..." << endl;
            continue;
        }else{
            //客户端有消息
            if(FD_ISSET(conn_fd,&rfds)){
                recv_num = recv(conn_fd,get_buff,sizeof(get_buff),0);
                if(recv_num < 0){
                    cout << "recv error!" << endl;
                    exit(1);
                }else{
                    get_buff[recv_num] = 0;
                }

                str = get_buff;
                if(str == "exit"){
                    exit(1);
                }else{
                    cout << "client msg: " << get_buff << endl;
                }
            }

            //服务端有输入
            if(FD_ISSET(0,&rfds)){
                cin >> send_buff;
                fun_res = send(conn_fd,send_buff,strlen(send_buff),0);
                if(fun_res < 0){
                    cout << "send error!" << endl;
                    exit(1);
                }
            }
        }
    }

    close(conn_fd);
    close(socket_fd);

    return 0;
}

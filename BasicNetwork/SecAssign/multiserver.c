#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <memory.h>
#include <errno.h>
#include "common.h"
#include <arpa/inet.h>
#include <sys/select.h>  // For fd_set and select()
#include <sys/types.h>          // For data types
#include <arpa/inet.h>          // For inet_addr()
#include <unistd.h>             // For close()
#include <string.h>             // For memset()
#include <sys/time.h>           // For struct timeval

#define MAX_CLIENT_SUPPORTED    32
#define SERVER_PORT     2000 /*Server process is running on this port no. Client has to send data to this port no*/
test_struct_t test_struct;
result_struct_t res_struct;
char data_buffer[1024];


static void
intitiaze_monitor_fd_set(int *monitored_fd_s){

    int i = 0;
    for(; i < MAX_CLIENT_SUPPORTED; i++)
        monitored_fd_s[i] = -1;
}


static int 
get_max_fd(int *monitored_fd_s){
    int mmax = 0;
    int i = 0;
    for(; i< MAX_CLIENT_SUPPORTED; i++){
        if(monitored_fd_s[i] > mmax)
            mmax = monitored_fd_s[i];
    }
    return mmax;
}

static void
remove_from_monitored_fd_set(int skt_fd, int *monitored_fd_s){

    int i = 0;
    for(; i < MAX_CLIENT_SUPPORTED; i++){

        if(monitored_fd_s[i] != skt_fd)
            continue;

        monitored_fd_s[i] = -1;
        break;
    }
}

static void
re_init_readfds(fd_set *fd_set_ptr, int *monitored_fd_s){
    FD_ZERO(fd_set_ptr);
    int i = 0;
    for(; i< MAX_CLIENT_SUPPORTED; i++){
        if(monitored_fd_s[i] != -1)
            FD_SET(monitored_fd_s[i], fd_set_ptr);
    }
}

static void
add_to_monitored_fd_set(int skt_fd, int *monitored_fd_s){

    int i = 0;
    for(; i < MAX_CLIENT_SUPPORTED; i++){

        if(monitored_fd_s[i] != -1)
            continue;   
        monitored_fd_s[i] = skt_fd;
        break;
    }
}

void
communicate()
{
int monitored_fd_set[32];
int master_socket_num;
int common_socket_num = 0;
int sent_recv_bytes;
fd_set readfds;
int addr_len = sizeof(struct sockaddr);
struct sockaddr_in server_addr, client_addr;

intitiaze_monitor_fd_set(monitored_fd_set);

if ((master_socket_num = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1){
    printf("no socket");
}
server_addr.sin_addr.s_addr = INADDR_ANY;
server_addr.sin_port = SERVER_PORT;
server_addr.sin_family = AF_INET;

if (bind(master_socket_num, (struct sockaddr *)&server_addr, addr_len) == -1){
    printf("socket bind fail");
}
if (listen(master_socket_num, 5)<0)  
    {
        printf("listen failed\n");
        return;
    }

add_to_monitored_fd_set(master_socket_num, monitored_fd_set);

while(1){

    re_init_readfds(&readfds, monitored_fd_set); 
    printf("blocked on select System call...\n");

    select(get_max_fd(monitored_fd_set) + 1, &readfds, NULL, NULL, NULL);

    if (FD_ISSET(master_socket_num, &readfds)){
        common_socket_num = accept(master_socket_num, (struct sockaddr *)&client_addr, &addr_len);
        printf("connected%d\n", common_socket_num);
        if(common_socket_num < 0){

                printf("accept error : errno = %d\n", errno);
                exit(0);
            }
        add_to_monitored_fd_set(common_socket_num, monitored_fd_set);
    }
    else{
        int i = 0, common_socket_num = -1;
        for(; i < MAX_CLIENT_SUPPORTED; i++){

            if(FD_ISSET(monitored_fd_set[i], &readfds)){
                printf("recv data from %d\n", monitored_fd_set[i]);
                common_socket_num = monitored_fd_set[i];
                memset(data_buffer, 0, sizeof(data_buffer));
                sent_recv_bytes = recvfrom(common_socket_num, (char *)data_buffer, sizeof(data_buffer), 0, (struct sockaddr *)&client_addr, &addr_len);
                test_struct_t *client_data = (test_struct_t *)data_buffer;
            
                if(client_data->a == 0 && client_data->b ==0){

                        close(common_socket_num);
                        remove_from_monitored_fd_set(common_socket_num, monitored_fd_set);
                        
                        break;
                }
                result_struct_t result;
                
                result.c = client_data->a + client_data->b;
                printf("result is %d\n", result.c);
                sent_recv_bytes = sendto(common_socket_num, (char *)&result, sizeof(result_struct_t), 0,
                            (struct sockaddr *)&client_addr, sizeof(struct sockaddr));
            }
            
        }

    }

}


}

int main(int argc, char **argv)
{
    communicate();
    return 0;
}
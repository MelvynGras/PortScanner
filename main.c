/*
** EPITECH PROJECT, 2022
** PORT_SCANNER
** File description:
** A multi threaded port scanner
*/

#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT_MAX 65535
#define THREAD_NUM 16
#define IP_ADDR "8.8.8.8"

typedef struct scan_info {
    int port_start;
    char *ip_addr;
} scan_info_t;

void *scan_thread(void *arg)
{
    int i = *(int *)arg + 1;
    for (unsigned int port = i; port < PORT_MAX; port += (THREAD_NUM)) {
        struct sockaddr_in target;
        target.sin_family = AF_INET;
        target.sin_addr.s_addr = inet_addr(IP_ADDR);
        target.sin_port = htons(port);
        
        struct timeval timeout; 
        timeout.tv_sec = 0;
        timeout.tv_usec = 28200;
        
        int socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        setsockopt(socket_fd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
        setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
        
        int conn = connect(socket_fd, (struct sockaddr *)&target, sizeof(target));
        if (conn == 0) {
            printf("(open) %d/tcp\n", port);
        }

        close(socket_fd);
    }
    pthread_exit(EXIT_SUCCESS);
}

pthread_t threads[THREAD_NUM];
int ports[THREAD_NUM];

int main(int argc, char *argv[])
{
    printf("Starting port scanner...\n");
    for (int thread_id = 0; thread_id < THREAD_NUM; thread_id++) {
        ports[thread_id] = thread_id;
        pthread_create(&threads[thread_id], NULL, scan_thread, &ports[thread_id]);
    }
    for (int i = 0; i < THREAD_NUM; i++) {
        pthread_join(threads[i], NULL);
    }
    return 0;
}

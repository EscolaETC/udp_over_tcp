#include <stdio.h>
#include <winsock2.h>

//gcc client.c -o client -lws2_32

int server_fd, tcp_server_fd, client_fd, received, tcp_client_fd, tcp_client_size;
DWORD tcp_client_id, udp_client_id;
struct sockaddr_in server_addr, client_addr, tcp_server_addr, tcp_client_addr;

int slice(char *dest, char *source, int start, int end){
    int i;
    for (i = start; i < end; i++){
        dest[i - start] = source[i];
    }
    dest[end - start] = '\0';

    return end - start;
}

int tcp_client(LPVOID lpParam){
    int client_size;
    unsigned int received, length;
    unsigned char buffer[2048];
    unsigned char send[2048];
    
    client_size = sizeof(client_addr);

    while (1){
        received = recv(tcp_server_fd, buffer, sizeof(buffer), 0);

        if (received == -1){
            continue;
        }

        if(received > 0){
            int i;
            int start = 0, end = 0;
            for (i = 0; i < received; i++){
                if (i + 4 > received){
                    break;
                }

                if (buffer[i] == 127 && buffer[i + 1] == 126 && buffer[i + 2] == 125 && buffer[i + 3] == 124 && buffer[i + 4] == 123){
                    length = slice(send, buffer, start, end);
                    //printf("Received -%d-%s-\n", length, send);
                    sendto(server_fd, send, length, 0, (const struct sockaddr *)&client_addr, client_size);
                    start = end + 5;
                }
                end++;
            }
        }
    }

    return 0;
}

int main(){
    WSADATA wsaData;

    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0){
        printf("WSAStartup failed with code %d\n", GetLastError());
        return 0;
    }

    tcp_server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    server_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (tcp_server_fd == INVALID_SOCKET || server_fd == INVALID_SOCKET){
        printf("socket failed with code %d\n", GetLastError());
        return 0;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(27000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    struct hostent* myHostent = gethostbyname("0.tcp.sa.ngrok.io");

    struct in_addr addr;
    addr.s_addr = *(u_long *) myHostent->h_addr_list[0];
    printf("%s\n", inet_ntoa(addr));

    tcp_server_addr.sin_family = AF_INET;
    tcp_server_addr.sin_addr.s_addr = inet_addr(inet_ntoa(addr));
    tcp_server_addr.sin_port = htons(11011);

    if (bind(server_fd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
        printf("bind failed with code %d\n", GetLastError());
        return 0;
    }

    if (connect(tcp_server_fd, (struct sockaddr *)&tcp_server_addr, sizeof(tcp_server_addr)) == SOCKET_ERROR){
        printf("connect failed with code %d\n", GetLastError());
        return 0;
    }

    CreateThread(NULL, 0,(LPTHREAD_START_ROUTINE) tcp_client, NULL, 0, &tcp_client_id);

    int client_size;
    unsigned int received;
    unsigned char buffer[2048];
    
    client_size = sizeof(client_addr);
    
    while (1){
        received = recvfrom(server_fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &client_size);

        if (received == -1){
            continue;
        }

        if (received > 0){
            buffer[received] = 127;
            buffer[received + 1] = 126;
            buffer[received + 2] = 125;
            buffer[received + 3] = 124;
            buffer[received + 4] = 123;
            //printf("Sent -%d-%s-\n", received + 5, buffer);
            send(tcp_server_fd, buffer, received + 5, 0);
        }
    }

    return 0;
}
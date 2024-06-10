#include <stdio.h>
#include <winsock2.h>

//gcc server.c -o server -lws2_32

int server_fd, udp_server_fd, client_fd, udp_client_fd, client_size;
DWORD tcp_server_id, udp_server_id;
struct sockaddr_in server_addr, udp_server_addr, client_addr, udp_client_addr;

int slice(char *dest, char *source, int start, int end){
    int i;
    for (i = start; i < end; i++){
        dest[i - start] = source[i];
    }
    dest[end - start] = '\0';

    return end - start;
}

int tcp_server(LPVOID lpParam){
    unsigned int received, length;
    unsigned char buffer[2048];
    unsigned char send[2048];
    
    while (1){
        received = recv(client_fd, buffer, sizeof(buffer), 0);

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
                    sendto(udp_server_fd, send, length, 0, (const struct sockaddr *)&udp_server_addr, sizeof(udp_server_addr));
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

    server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    udp_server_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (server_fd == INVALID_SOCKET || udp_server_fd == INVALID_SOCKET){
        printf("socket failed with code %d\n", GetLastError());
        return 0;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8000);

    //131.196.198.52:27645
    //192.168.100.44:27015
    //131.196.198.52:27848

    udp_server_addr.sin_family = AF_INET;
    udp_server_addr.sin_addr.s_addr = inet_addr("131.196.198.52");
    udp_server_addr.sin_port = htons(27848);

    if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == INVALID_SOCKET){
        printf("bind failed with code %d\n", GetLastError());
        return 0;
    }

    if(listen(server_fd, 10) < 0){
        printf("listen failed with code %d\n", GetLastError());
        return 0;
    }

    int connected = 0;

    int udp_client_size;
    unsigned int received;
    unsigned char buffer[2048];

    while (1){
        if (connected == 0){
            client_size = sizeof(client_addr);
            client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_size);

            if (client_fd == INVALID_SOCKET){
                continue;
            }

            printf("Client connected\n");

            CreateThread(NULL, 0,(LPTHREAD_START_ROUTINE) tcp_server, NULL, 0, &tcp_server_id);

            connected = 1;
        }

        udp_client_size = sizeof(udp_server_addr);
        received = recvfrom(udp_server_fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&udp_server_addr, &udp_client_size);

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
            send(client_fd, buffer, received + 5, 0);
        }
    }

    return 0;
}
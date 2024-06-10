#include <stdio.h>
#include <string.h>
#include <WinSock2.h>
#include <Windows.h>

//gcc tc.c -o tc -lws2_32

int server_fd, game_client_size, sent, game_server_fd, game_server_size;
DWORD game_client_id, game_server_id;
struct sockaddr_in server_addr, game_client_addr, game_server_addr;

int game_server(LPVOID lpParam){
    char buffer[2048];
    int received;

    game_server_size = sizeof(game_server_addr);
    
    while (1){
        received = recvfrom(game_server_fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&game_server_addr, &game_server_size);

        if (received > 0){
            printf("Received from game server: -%d-%s-\n", received, buffer);
            sendto(server_fd, buffer, received, 0, (const struct sockaddr *)&game_client_addr, sizeof(game_client_addr));
        }
    }
}

int main(){
    WSADATA wsaData;

    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0){
        printf("WSAStartup failed with code %d\n", GetLastError());
        return 0;
    }

    server_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    game_server_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(27001);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    //131.196.198.196:27069
    //192.168.100.44:27015
    //131.196.198.52:27645

    game_server_addr.sin_family = AF_INET;
    game_server_addr.sin_port = htons(27015);
    game_server_addr.sin_addr.s_addr = inet_addr("192.168.100.44");

    bind(server_fd, (const struct sockaddr *)&server_addr, sizeof(server_addr));
    
    CreateThread(NULL, 0,(LPTHREAD_START_ROUTINE) game_server, NULL, 0, &game_server_id);

    char buffer[2048];
    int received;

    game_client_size = sizeof(game_client_addr);
    
    while (1){
        received = recvfrom(server_fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&game_client_addr, &game_client_size);

        if (received > 0){
            printf("Received from game client: -%d-%s-\n", received, buffer);
            sent = sendto(game_server_fd, buffer, received, 0, (const struct sockaddr *)&game_server_addr, sizeof(game_server_addr));
            Sleep(2000);
        }
    }
}
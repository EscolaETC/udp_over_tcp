#include <stdio.h>
#include <string.h>

void send(char *data, int size){
    printf("Send: %s %d\n", data, size);
}

void slice(char *dest, char *source, int start, int end){
    int i;
    for (i = start; i < end; i++){
        dest[i - start] = source[i];
    }
    dest[end - start] = '\0';
}

int main(){
    char packet[2048] = "hello world";

    packet[strlen(packet)] = 127;

    int packet_length = strlen(packet);

    printf("%c\n", 127);

    strcat(packet, "~@;'+]");
    strcat(packet, "new world");
    strcat(packet, "~@;'+]");

    printf("%s\n", packet);

    int packet_length_final = strlen(packet);

    char buffer[2048];

    int i, start = 0, end = 0;
    for (i = 0; i < packet_length_final; i++){
        if (i + 2 > packet_length_final){
            break;
        }

        if (packet[i] == '~' && packet[i + 1] == '@' && packet[i + 2] == ';' && packet[i + 3] == '\'' && packet[i + 4] == '+' && packet[i + 5] == ']'){
            printf("%d %d\n", start, end);
            slice(buffer, packet, start, end);
            send(buffer, end - start);
            start = end + 6;
        }
        end++;
    }

    return 0;
}
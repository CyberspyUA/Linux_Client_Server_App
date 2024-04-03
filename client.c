#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
const int port = 8080;
const int bufferSize = 1024;

int main() 
{
    struct sockaddr_in serverAddress;
    char buffer[bufferSize];
    memset(buffer, 0, bufferSize);
    /*
    * Створення дескриптора файлу сокета
    * (Create socket file descriptor)
    */ 
    int socketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFileDescriptor < 0) 
    {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);

    /*
    * Перетворення IPv4 та IPv6 адрес з текстової форми в бінарну
    * (Convert IPv4 and IPv6 addresses from text to binary form)
    */ 
    if(inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr)<=0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    // Підключитись до сервера (Connect to server)
    if (connect(socketFileDescriptor, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) 
    {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server\n");

    // Відправка повідомлень на сервер (Send messages to server)
    while(1) 
    {
        printf("Enter message: ");
        fgets(buffer, bufferSize, stdin);

        // Надіслати повідомлення на сервер (Send message to server)
        send(socketFileDescriptor, buffer, strlen(buffer), 0);

        // Отримуємо відповідь від сервера (Receive response from server)
        int serverResponse = read(socketFileDescriptor, buffer, bufferSize);
        printf("Server response: %s\n", buffer);

        // Очищення буфера (Clear buffer)
        memset(buffer, 0, bufferSize);

        // Перевіряємо наявність команди роз'єднання (Check for disconnect command)
        if (strcmp(buffer, "disconnect\n") == 0)
            break;
    }
    close(socketFileDescriptor);
    return 0;
}

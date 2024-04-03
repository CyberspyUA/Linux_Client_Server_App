#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

const int port = 8080;
const int maxClients = 10;
const int bufferSize = 1024;

/*
* Функція для обробки кожного клієнтського з'єднання
* (Function to handle each client connection)
*/ 
void *HandleClient(void *arg) 
{
    /*
    * Приведення аргументу до вказівника int для отримання клієнтського сокету
    * (Cast argument to int pointer to get client socket)
    */  
    int clientSocket = *((int *)arg); 
    char buffer[bufferSize]; // Буфер для отримання даних від клієнта (Buffer for receiving data from client)
    memset(buffer, 0, bufferSize);
    int valueRead; // Змінна для зберігання кількості прочитаних байт (Variable to store the number of bytes read)

    /*
    * Цикл для безперервного отримання даних від клієнта
    * (Loop to continuously receive data from client)
    */ 
    while ((valueRead = read(clientSocket, buffer, bufferSize)) > 0) 
    {
        printf("Received: %s", buffer);

        /*
        * Перевіряємо вміст повідомлення та реагуємо відповідно
        * (Check message content and respond accordingly)
        */ 
        if (strcmp(buffer, "hello\n") == 0) 
        {
            char response[] = "world\n";
            send(clientSocket, response, strlen(response), 0);
        } 
        else 
        {
            send(clientSocket, buffer, strlen(buffer), 0);
        }

        /*
        * Очищення буфера для наступного повідомлення
        * Clear buffer for next message
        */ 
        memset(buffer, 0, bufferSize);
    }

    printf("Client disconnected\n");
    close(clientSocket); // Закриваємо клієнтський сокет (Close client socket)
    pthread_exit(NULL); // Виходимо з потоку (Exit the thread)
}

int main() 
{
    int serverFileDescriptor, clientSocket; // Змінні для серверних та клієнтських сокетів (Variables for server and client sockets)
    struct sockaddr_in address; // Структура для адреси сокету (Structure for socket address)
    int addrlen = sizeof(address); // Розмір структури адрес сокетів (Size of socket address structure)

    // Створення дескриптора файлу сокета (Create socket file descriptor)
    if ((serverFileDescriptor = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    /*
    * Налаштування параметрів сокета, щоб забезпечити кілька підключень
    * Set socket options to allow multiple connections
    */ 
    if (setsockopt(serverFileDescriptor, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0) 
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    /*
    * Налаштування адресної структури
    * (Configure address structure)
    */ 
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    /*
    * Підключити сокет до localhost
    * (Bind the socket to localhost)
    */ 
    if (bind(serverFileDescriptor, (struct sockaddr *)&address, sizeof(address)) < 0) 
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    /*
    * Слухати вхідні з'єднання
    * (Listen for incoming connections)
    */ 
    if (listen(serverFileDescriptor, 3) < 0) 
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", port);

    /*
    * Основний цикл сервера для прийому вхідних з'єднань
    * (Main server loop to accept incoming connections)
    */ 
    while (1) 
    {
        /*
        * Приймаємо вхідне з'єднання
        * (Accept incoming connection)
        */ 
        if ((clientSocket = accept(serverFileDescriptor, 
                                   (struct sockaddr *)&address, 
                                   (socklen_t*)&addrlen)) < 0) 
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        printf("New client connected\n");

        /*
        * Створюємо потік для роботи з клієнтом
        * (Create thread to handle client)
        */ 
        pthread_t tid; // Змінна ID потоку (Thread ID variable)
        if (pthread_create(&tid, NULL, HandleClient, &clientSocket) != 0) 
        {
            perror("pthread_create");
            close(clientSocket);
        }
    }

    close(serverFileDescriptor); // Закриваємо серверний сокет (Close server socket)
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    const char *response = 
        "HTTP/1.1 200 OK\n"
        "Content-Type: text/html\n\n"
        "<html><body><h1>Hello, World!</h1></body></html>";

    // Step 1: Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Step 2: Bind the socket to the address and port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Step 3: Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Server is listening on port %d\n", PORT);

    // Step 4: Accept and handle incoming connections
    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("Accept failed");
            continue;
        }

        // Read the request (not used here, but for HTTP parsing if needed)
        read(new_socket, buffer, BUFFER_SIZE);
        printf("Request received:\n%s\n", buffer);

        // Send response
        send(new_socket, response, strlen(response), 0);
        printf("Response sent\n");

        // Close connection
        close(new_socket);
    }

    // Cleanup
    close(server_fd);
    return 0;
}

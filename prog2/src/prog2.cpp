#include "../../lib/include/lib.hpp"

#include <iostream>
#include <cstring>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

class Server {
private:
    int server_fd;
    sockaddr_in address;
    const int port;

public:
    explicit Server(int port) : port(port), server_fd(-1) {}

    ~Server() {
        if (server_fd >= 0) close(server_fd);
    }

    void start() {
        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        
        if (server_fd == 0) {
            perror("Socket failed");
            exit(EXIT_FAILURE);
        }

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
            perror("Bind failed");
            exit(EXIT_FAILURE);
        }

        if (listen(server_fd, 3) < 0) {
            perror("Listen failed");
            exit(EXIT_FAILURE);
        }
        std::cout << "Server is listening on port " << port << std::endl;

        acceptConnections();
    }

private:
    void acceptConnections() {
        char buffer[1024] = {0};
        while (true) {
            int addrlen = sizeof(address);
            int client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
            
            if (client_socket < 0) {
                perror("Accept failed");
                continue;
            }

            while (true) {
                ssize_t bytes_received = read(client_socket, buffer, sizeof(buffer) - 1);
            
                if (bytes_received > 0) {
                    std::string str(buffer);
                    std::cout << "Received: " << str << std::endl;
                
                    if (library::f3(str)) {
                        std::cout << "Received data!" << std::endl;
                    } else {
                        std::cerr << "Error data!" << std::endl;
                    }

                    memset(buffer, 0, 1024);  

                } else {
                    std::cout << "No data received or connection closed by client." << std::endl;
                    close(client_socket);
                    break;
                }
            }
        }
    }
};

int main() {
    Server server(55555);
    server.start();
    return 0;
}


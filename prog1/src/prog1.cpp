#include "../../lib/include/lib.hpp"

#include <cerrno>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>
#include <queue>
#include <regex>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>

class Client {
private:
    int client_fd;
    sockaddr_in server_address;
    const std::string server_ip;
    const int server_port;

public:
    Client(const std::string &ip, int port) : server_ip(ip), server_port(port), client_fd(-1) {}

    ~Client() {
        if (client_fd >= 0) close(client_fd);
    }

    void start() {
        client_fd = socket(AF_INET, SOCK_STREAM, 0);
        
        if (client_fd < 0) {
            perror("Socket creation failed");
            exit(EXIT_FAILURE);
        }
        
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(server_port);
        
        if (inet_pton(AF_INET, server_ip.c_str(), &server_address.sin_addr) <= 0) {
            perror("Invalid address/ Address not supported");
            exit(EXIT_FAILURE);
        }

        if (connect(client_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
            perror("Connection failed");
        } else {
            std::cout << "Connected to server at " << server_ip << ":" << server_port << std::endl;
        }
    }

    void sendData(size_t message) {
        std::string str = std::to_string(message);
        if (send(client_fd, str.c_str(), str.length(), MSG_NOSIGNAL) < 0) {
            perror("Send failed");
            close(client_fd);
            start();
        }
    }
};

Client client("127.0.0.1", 55555); 

class Buffer {
private:
    std::queue<std::string> qu;
    std::mutex mtx;
    std::condition_variable cv;

public:
    void put(const std::string& value) {
        std::unique_lock<std::mutex> lock(mtx);
        qu.push(value);
        cv.notify_all();   
    }

    std::string get() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this]() { return !qu.empty(); }); 
        std::string value = qu.front(); 
        qu.pop();
        return value;
    }
};

void producer(Buffer& buffer) {
    while (true) {
        std::string input = std::string();
        std::cout << "Enter a numeric string (max 64 chars): ";
        std::getline(std::cin, input);

        if (input.size() > 64 || !std::regex_match(input, std::regex("\\d+"))) {
            std::cerr << "Invalid input. String must be numeric and <= 64 characters.\n";
            continue;
        }

        library::f1(input);
        buffer.put(input);
    }
}

void consumer(Buffer& buffer) {
    while (true) {
        std::string data = buffer.get();
        size_t result = library::f2(data);

        std::cout << "Consumer received: " << data << std::endl;
        std::cout << "Result: " << result << std::endl;
        
        client.sendData(result);
    }
}

int main() {
    Buffer buffer;

    client.start();

    std::thread producer_thread(producer, std::ref(buffer));
    std::thread consumer_thread(consumer, std::ref(buffer));

    producer_thread.join();
    consumer_thread.join();

    return 0;
}


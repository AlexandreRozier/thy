//
// Created by thyriaen on 11/01/18.
//

#include "Networking.h"
#include <iostream>
#include <cstring>
#include <cstring>
#include <netdb.h>
#include <arpa/inet.h>

Networking::Networking() {
    UDT::startup();
    createSocket(&this->socket);
}

Networking::~Networking() {
    std::cout << "[netwo] :: closing the socket" << std::endl;
    UDT::close(this->socket);
    UDT::cleanup();
}

int Networking::createSocket(UDTSOCKET *socket) {

    struct addrinfo hints{}, *local;
    memset(&hints, 0, sizeof(hints));

    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (0 != getaddrinfo(nullptr, "9009", &hints, &local)) {
        std::cout << "[error] :: incorrect local address" << std::endl;
        return -1;
    }

    *socket = UDT::socket(local->ai_family, local->ai_socktype, local->ai_protocol);
    std::cout << "[netwo] :: socket created successfully" << std::endl;

    freeaddrinfo(local);

    return 0;
}

int Networking::connectTo(const char &addr, const char &port) {

    struct addrinfo hints{}, *server;
    memset(&hints, 0, sizeof(hints));

    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (0 != getaddrinfo(&addr, &port, &hints, &server)) {
        std::cout << "[error] :: incorrect server address: " << &addr << ":" << &port << std::endl;
        return -1;
    }

    if (UDT::ERROR == UDT::connect(this->socket, server->ai_addr, server->ai_addrlen)) {
        std::cout << "[error] :: connecting: " << UDT::getlasterror().getErrorMessage() << std::endl;
        return -1;
    }

    std::cout << "[netwo] :: connection with " << &addr << ":" << &port << " established" << std::endl;

    freeaddrinfo(server);
    return 0;
}

int Networking::sendData(const char &data) {
    if( UDT::ERROR == UDT::send(this->socket, &data, 12*sizeof(char), 0)) {
        std::cout << "[error] :: send:" << UDT::getlasterror().getErrorMessage() << std::endl;
        return -1;
    }
    std::cout << "[netwo] :: sending "<< &data << " successful" << std::endl;
    return 0;
}

int Networking::listen(const int port) {

    addrinfo hints{}, *local;
    memset(&hints, 0, sizeof(hints));

    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (0 != getaddrinfo(nullptr, std::to_string(port).c_str(), &hints, &local)) {
        std::cout << "[error] :: illegal port number or port is busy" << std::endl;
        return -1;
    }

    if (UDT::ERROR == UDT::bind(this->socket, local->ai_addr, local->ai_addrlen)) {
        std::cout << "[error] :: bind: " << UDT::getlasterror().getErrorMessage() << std::endl;
        return -1;
    }

    freeaddrinfo(local);

    if (UDT::ERROR == UDT::listen(this->socket, 10)) {
        std::cout << "[error] :: listen: " << UDT::getlasterror().getErrorMessage() << std::endl;
        return -1;
    }

    std::cout << "[netwo] :: listening on: " << port << std::endl;

    return 0;
}

int Networking::connectionHandler() {

    while (true) {
        sockaddr_storage client{};
        int addrlen = sizeof(client);

        UDTSOCKET recv;
        if (UDT::INVALID_SOCK == (recv = UDT::accept(this->socket, (sockaddr*)&client, &addrlen))) {
            std::cout << "[error] :: accept: " << UDT::getlasterror().getErrorMessage() << std::endl;
            return 0;
        }

        char clienthost[NI_MAXHOST];
        char clientservice[NI_MAXSERV];
        getnameinfo((sockaddr *)&client, addrlen, clienthost, sizeof(clienthost), clientservice,
                    sizeof(clientservice), NI_NUMERICHOST|NI_NUMERICSERV);
        std::cout << "[netwo] :: new connection: " << clienthost << ":" << clientservice << std::endl;

        pthread_t rcvthread;
        pthread_create(&rcvthread, nullptr, recvdata, new UDTSOCKET(recv));
        pthread_detach(rcvthread);
    }

    return 0;
}

void* recvdata(void* usocket) {
    UDTSOCKET recver = *(UDTSOCKET*)usocket;
    delete (UDTSOCKET*)usocket;
    char data[100];

    if (UDT::ERROR == UDT::recv(recver, data, 100, 0)) {
        std::cout << "[error] :: receive:" << UDT::getlasterror().getErrorMessage() << std::endl;
    }

    std::cout << "[outpu] :: " << data << std::endl;
    return nullptr;
}
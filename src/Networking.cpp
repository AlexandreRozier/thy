//
// Created by thyriaen on 11/01/18.
//

#include "Networking.h"
#include <iostream>
#include <cstring>
#include <netdb.h>

Networking::Networking() {
    createSocket(&this->socket);
}

Networking::~Networking() {
    std::cout << "[netwo] :: closing the socket" << std::endl;
    UDT::close(this->socket);
}

int Networking::createSocket(UDTSOCKET *socket) {

    struct addrinfo hints, *local;
    memset(&hints, 0, sizeof(hints));

    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (0 != getaddrinfo(nullptr, "9000", &hints, &local)) {
        std::cout << "[error] :: incorrect local address" << std::endl;
        return -1;
    }

    *socket = UDT::socket(local->ai_family, local->ai_socktype, local->ai_protocol);
    std::cout << "[netwo] :: socket created successfully" << std::endl;

    freeaddrinfo(local);

    return 0;
}

int Networking::connectToServer(const char &addr, const char &port) {

    struct addrinfo hints, *server;
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


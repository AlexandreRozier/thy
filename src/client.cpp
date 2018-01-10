#include <iostream>
#include <udt/udt.h>
#include <arpa/inet.h>
#include <cstring>

int main() {
    UDTSOCKET client = UDT::socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(9000);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    memset(&(serv_addr.sin_zero), '\0', 8);

    /*
    bool rendezvous = true;
    UDT::setsockopt(client, 0, UDT_RENDEZVOUS, &rendezvous, sizeof(rendezvous));
    */

    // connect to the server, implict bind
    if (UDT::ERROR == UDT::connect(client, (sockaddr*)&serv_addr, sizeof(serv_addr))) {
        std::cout << "[CON] :: " << UDT::getlasterror().getErrorMessage();
        return 0;
    }

    char* hello = (char*) "hello world!\n";
    if (UDT::ERROR == UDT::send(client, hello, strlen(hello) + 1, 0)) {
        std::cout << "[SEN] :: " << UDT::getlasterror().getErrorMessage();
        return 0;
    }

    UDT::close(client);

    return 1;
}
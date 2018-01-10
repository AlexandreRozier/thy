#include <arpa/inet.h>
#include <udt/udt.h>
#include <iostream>
#include <cstring>

int main() {

    UDTSOCKET serv = UDT::socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in my_addr;
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(9000);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    memset(&(my_addr.sin_zero), '\0', 8);

    if (UDT::ERROR == UDT::bind(serv, (sockaddr*)&my_addr, sizeof(my_addr))) {
        std::cout << "bind: " << UDT::getlasterror().getErrorMessage();
        return 0;
    }

    UDT::listen(serv, 10);

    int namelen;
    sockaddr_in their_addr;

    UDTSOCKET recver = UDT::accept(serv, (sockaddr*)&their_addr, &namelen);

    std::cout << "new connection: " << inet_ntoa(their_addr.sin_addr) << ":" << ntohs(their_addr.sin_port) << std::endl;

    char data[100];

    if (UDT::ERROR == UDT::recv(recver, data, 100, 0)) {
        std::cout << "recv:" << UDT::getlasterror().getErrorMessage() << std::endl;
        return 0;
    }

    std::cout << data << std::endl;

    UDT::close(recver);
    UDT::close(serv);

    return 1;
}

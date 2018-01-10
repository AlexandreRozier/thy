#include <iostream>
#include <udt/udt.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <cstdlib>
#include <netdb.h>
#include <iostream>
#include <udt/ccc.h>

void* monitor(void*);

int main(int argc, char* argv[]) {

    if ((3 != argc) || (0 == atoi(argv[2]))) {
        std::cout << "[usage] :: " << argv[0] << " serverIp serverPort" << std::endl;
        return -1;
    }

    struct addrinfo hints, *local, *server;
    memset(&hints, 0, sizeof(hints));

    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (0 != getaddrinfo(nullptr, "9000", &hints, &local)) {
        std::cout << "[error] :: incorrect local address.\n" << std::endl;
        return -1;
    }

    UDTSOCKET sock = UDT::socket(local->ai_family, local->ai_socktype, local->ai_protocol);

    freeaddrinfo(local);

    if (0 != getaddrinfo(argv[1], argv[2], &hints, &server)) {
        std::cout << "[error] :: incorrect server address: " << argv[1] << ":" << argv[2] << std::endl;
        return -1;
    }

    if (UDT::ERROR == UDT::connect(sock, server->ai_addr, server->ai_addrlen)) {
        std::cout << "[error] :: connecting: " << UDT::getlasterror().getErrorMessage() << std::endl;
        return -1;
    }

    freeaddrinfo(server);

    char *data = (char*) "Hello World\n";

    if( UDT::ERROR == UDT::send(sock, data, 13*sizeof(char), 0)) {
        std::cout << "[error] :: send:" << UDT::getlasterror().getErrorMessage() << std::endl;
        return -1;
    }

    UDT::close(sock);

    std::cout << "[statu] :: success" << std::endl;
    return 0;
}




/*

    UDTSOCKET client = UDT::socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(9000);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    memset(&(serv_addr.sin_zero), '\0', 8);


    bool rendezvous = true;
    UDT::setsockopt(client, 0, UDT_RENDEZVOUS, &rendezvous, sizeof(rendezvous));


    // connect to the server, implict bind
    if (UDT::ERROR == UDT::connect(client, (sockaddr*)&serv_addr, sizeof(serv_addr))) {
        std::cout << "[CON] :: " << UDT::getlasterror().getErrorMessage();
        return -1;
    }

    char* hello = (char*) "hello world!\n";
    if (UDT::ERROR == UDT::send(client, hello, strlen(hello) + 1, 0)) {
        std::cout << "[SEN] :: " << UDT::getlasterror().getErrorMessage();
        return -1;
    }

*/

void* monitor(void* s) {

    UDTSOCKET u = *(UDTSOCKET*)s;

    UDT::TRACEINFO perf;

    std::cout << "SendRate(Mb/s)\tRTT(ms)\tCWnd\tPktSndPeriod(us)\tRecvACK\tRecvNAK" << std::endl;

    while (true) {
        sleep(1);

        if (UDT::ERROR == UDT::perfmon(u, &perf)) {
            std::cout << "perfmon: " << UDT::getlasterror().getErrorMessage() << std::endl;
            break;
        }

        std::cout << perf.mbpsSendRate << "\t\t"
                  << perf.msRTT << "\t"
                  << perf.pktCongestionWindow << "\t"
                  << perf.usPktSndPeriod << "\t\t\t"
                  << perf.pktRecvACK << "\t"
                  << perf.pktRecvNAK << std::endl;
    }

    return nullptr;

}

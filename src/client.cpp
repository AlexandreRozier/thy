#include "Networking.h"
#include <iostream>
#include <cstring>
#include <unistd.h>


int main(int argc, char* argv[]) {

    if ((3 != argc) || (0 == atoi(argv[2]))) {
        std::cout << "[usage] :: " << argv[0] << " serverIp serverPort" << std::endl;
        return -1;
    }

    Networking net = Networking();

    if(net.connectToServer(*argv[1], *argv[2])) {
        return -1;
    }

    char *data = (char*) "Hello World";
    if(net.sendData(*data)) {
        return -1;
    }

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

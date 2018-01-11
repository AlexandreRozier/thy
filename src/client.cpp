#include "Networking.h"
#include <iostream>
#include <cstring>
#include <unistd.h>

int main(int argc, char* argv[]) {

    if (argc > 2) {
        std::cout << "[usage] :: " << argv[0] << " [serverIp]" << std::endl;
        return -1;
    }

    std::string server;
    argc == 1 ? server = "ec2-18-220-56-40.us-east-2.compute.amazonaws.com" : server = argv[1];

    Networking net = Networking();

    std::string port("9009");
    if(net.connectTo(*server.c_str(), *port.c_str())) {
        return -1;
    }

    std::string data("Hello World");
    if(net.sendData(*data.c_str())) {
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

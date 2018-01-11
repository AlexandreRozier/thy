#include "Networking.h"

void* recvdata(void* usocket);

int main() {

    Networking net = Networking();
    net.listen(9009);

    net.connectionHandler();

    return 0;
}



/*
void* recvdata(void* usocket) {
    UDTSOCKET recver = *(UDTSOCKET*)usocket;
    delete (UDTSOCKET*)usocket;

    char* data;
    int size = 100000;
    data = new char[size];

    while (true) {
        int rsize = 0;
        int rs;
        while (rsize < size) {

            int rcv_size;
            int var_size = sizeof(int);
            UDT::getsockopt(recver, 0, UDT_RCVDATA, &rcv_size, &var_size);
            if (UDT::ERROR == (rs = UDT::recv(recver, data + rsize, size - rsize, 0))) {
                std::cout << "recv:" << UDT::getlasterror().getErrorMessage() << std::endl;
                break;
            }

            rsize += rs;
        }

        if (rsize < size) break;
    }

    delete [] data;
    UDT::close(recver);

    return nullptr;
}
*/
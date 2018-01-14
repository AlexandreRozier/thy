#ifndef THY_NETWORKING_H
#define THY_NETWORKING_H

#include <udt/udt.h>

class Networking {
  public:
    Networking();
    ~Networking();

    /**
    * Creates a local UDT socket to be used
    * @param socket pointer to an UDTSOCKET
    * @return 0 iff successful
    */
    int createSocket(UDTSOCKET *socket);

    /**
    * Connects to a server on a socket
    * @param socket reference to a socket
    * @param addr of the server to connect to
    * @param port of the server to connect to
    * @return 0 iff successful
    */
    int connectTo(const char &addr, const char &port);

    /**
     * Sends data over the socket
     * @param socket has to be connected already
     * @param data reference to a char array
     * @return 0 iff successful
     */
    int sendData(const char &data);

    /**
     * binds the socket and listens on given port
     * @param port to listen on
     * @return 0 iff successful
     */
    int listen(int port);

    int connectionHandler(void *function(void*));
  private:
    UDTSOCKET socket;
};

void* recvdata(void* usocket);

#endif //THY_NETWORKING_H
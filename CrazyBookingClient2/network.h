#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include <string>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>

using namespace std;

#define SIZE 1024

class Network
{
public:
    Network();
    ~Network();
    Network(int port): port(port) {
    }

    int getSocket() const;

    int getPort() const;
    void setPort(int newPort);

    bool connectToServer();
    bool connectToServer(int newPort);

    bool sendRequest(const char *request, int sizeRequest, char *out_Response, int &out_sizeResponse) const;

    void closeConnection();

private:
    int sock;
    int port = 8080;
    bool connected = false;
    sockaddr_in hint;
};

#endif // NETWORK_H

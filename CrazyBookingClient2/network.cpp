#include "network.h"

Network::Network()
{

}

Network::~Network() {
    close(sock);
}

int Network::getSocket() const
{
    return sock;
}

int Network::getPort() const
{
    return port;
}

void Network::setPort(int newPort)
{
    port = newPort;
}

bool Network::connectToServer()
{
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock==-1){
        cout << "Cannot create the socket... Aborting.\n";
        return false;
    }
    //    Create a hint structure
        string ipAddress = "127.0.0.1";

        hint.sin_family = AF_INET;
        hint.sin_port = htons(port);
        inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

    //    Connect to the server on the socket
        int connectRes = -1;
        for(int i=0; i<10 && connectRes==-1; ++i) {
            connectRes = connect(sock, (sockaddr*)&hint, sizeof(hint));
            if (connectRes==-1) {
                perror("Could not connect to the server");
                if (i==9) {
                    cout << "ABORTING\n";
                    return false;
                }
                cout << "Trying to reconnect in 5 seconds...\n\n";
                this_thread::sleep_for(chrono::milliseconds(5000));
            }
        }

    //  GETTING SERVER GREETINGS
        char buffer[SIZE];
        memset(buffer, 0, SIZE);
        int bytesReceived = recv(sock, buffer, SIZE, 0);
        cout << "SERVER> " << string(buffer,bytesReceived) << "\r\n";
        return connected = true;
}








bool Network::connectToServer(int newPort) {
    setPort(newPort);
    return connectToServer();
}

bool Network::sendRequest(const char *request, int sizeRequest, char *out_Response, int &out_sizeResponse) const {
    if (!connected) {
        cout << "NETWORK > Request cannot be sent: no connection!\n";
        return false;
    }
    int sendRes = send(sock, request, sizeRequest +1, 0);
    if (sendRes == -1) {
        cout << "SERVER > Could not get the request!\r\n";
        return false;
    }
    memset(out_Response, 0, SIZE);
    out_sizeResponse = recv(sock, out_Response, SIZE, 0);
    if (out_sizeResponse<1)
        return false;
    return true;
}

void Network::closeConnection() {
    close(sock);
    connected = false;
}

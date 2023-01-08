//Multiple socket connections handled with select and fd_set on Linux

#ifndef CRAZYBOOKINGSERVER_H
#define CRAZYBOOKINGSERVER_H

#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h> //close
#include <arpa/inet.h> //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros

#include <iostream>
#include <signal.h>

#include "bookinghandler.h"
#include "../common_definitions.h"

#define TRUE 1
#define FALSE 0

#define MAX_CLIENTS 250


class CrazyBookingServer
{
public:
    CrazyBookingServer(const char* databaseFileName);
    void run();

private:
    BookingHandler bookingHandler;
    fd_set readfds; //Set of socket descriptors
    int master_socket;
    int client_socket[MAX_CLIENTS];
    struct sockaddr_in address;

    char buffer[SIZE];
};

#endif // CRAZYBOOKINGSERVER_H

#include "crazybookingserver.h"

CrazyBookingServer::CrazyBookingServer(const char *databaseFileName)
    :bookingHandler(databaseFileName)
{
    bookingHandler.printPlacesDistribution();

    signal(SIGPIPE, SIG_IGN);   //Ignores crash when connection closes SIGPIPE

    int opt = TRUE;

    //initialise all client_socket[] to 0 so not checked
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        client_socket[i] = 0;
    }

    //create a master socket
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
    {
        perror("Socket failure");
        exit(EXIT_FAILURE);
    }

    //set master socket to allow multiple connections , not necessary
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
        sizeof(opt)) < 0 )
    {
        perror("setsockopt error");
        exit(EXIT_FAILURE);
    }

    //type of socket created
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    //bind the socket to localhost port 8080
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("Could not bind");
        exit(EXIT_FAILURE);
    }
    printf("Listener on port %d \n", PORT);

    //try to specify maximum of 250 pending connections for the master socket
    if (listen(master_socket, MAX_CLIENTS) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }



}









void CrazyBookingServer::run()
{
    int max_sd, sd;
    int activity;
    int new_socket;
    int valread;
    int addrlen;

    addrlen = sizeof(address);
    puts("Waiting for connections ...");

    while(TRUE)
    {
        //clear the socket set
        FD_ZERO(&readfds);

        //add master socket to set
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;

        //add child sockets to set
        for (int i = 0 ; i < MAX_CLIENTS ; i++)
        {
            //socket descriptor
            sd = client_socket[i];

            //if valid socket descriptor then add to read list
            if(sd > 0)
                FD_SET( sd , &readfds);

            //highest file descriptor number, need it for the select function
            if(sd > max_sd)
                max_sd = sd;
        }

        //wait for an activity on one of the sockets , timeout is NULL ,
        //so wait indefinitely
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);

        if ((activity < 0) && (errno!=EINTR))
        {
            printf("Error in select");
        }

        //If something happened on the master socket ,
        //then its an incoming connection
        if (FD_ISSET(master_socket, &readfds))
        {
            if ((new_socket = accept(master_socket,
                    (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            //inform user of socket number - used in send and receive commands
            printf("New connection , socket fd is %d , ip is : %s , port : %d\n" , new_socket , inet_ntoa(address.sin_addr) , ntohs
                (address.sin_port));

            //send new connection greeting message
            const char *welcomeMessage = "Hi! You are connected to the service CrazyBooking \r\n";
            if( send(new_socket, welcomeMessage, strlen(welcomeMessage), 0) != strlen(welcomeMessage) )
            {
                perror("send");
            }

            puts("Welcome message sent successfully");

            //add new socket to array of sockets
            for (int i = 0; i < MAX_CLIENTS; i++)
            {
                //if position is empty
                if( client_socket[i] == 0 )
                {
                    client_socket[i] = new_socket;
                    printf("Adding to list of sockets as %d\n" , i);

                    break;
                }
            }
        }

        //else its some IO operation on some other socket
        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            sd = client_socket[i];

            if (FD_ISSET( sd , &readfds))
            {
                //Check if it was for closing , and also read the
                //incoming message
                if ((valread = read( sd , buffer, 1024)) == 0)
                {
                    //Somebody disconnected , get his details and print
                    getpeername(sd , (struct sockaddr*)&address , \
                        (socklen_t*)&addrlen);
                    printf("Host disconnected , ip %s , port %d \n" ,
                        inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

                    //Close the socket and mark as 0 in list for reuse
                    close( sd );
                    client_socket[i] = 0;
                }

                //Echo back the message that came in
                else
                {
                    //set the string terminating NULL byte on the end
                    //of the data read
                    buffer[valread] = '\0';
                    cout << buffer << endl;
                    char command[5];

                    sscanf(buffer,"%4s%3s",command,buffer);
                    cout << "TRYING TO>" << command << "," << buffer << endl;

                    if (strcmp(command, FREE_LIST_PLACES_REQUEST)==0){
                        const char *freePlaces = bookingHandler.getFreePlacesList();
                        cout << freePlaces << endl;
                        cout << strlen(freePlaces) << endl << endl;
                        send(sd, freePlaces, strlen(freePlaces)+1,0);
                    }
                    if (strcmp(command, BOOK_REQUEST)==0){
                        Place place;
                        place.row = buffer[0];
                        if (buffer[2] == '\r' || buffer[2] == 0)
                            place.col = buffer[1] - '0';
                        else{
                            place.col = (buffer[1]-'0')*10 + buffer[2] - '0';
                        }
                        cout << "Trying to book place "; place.print(); cout <<endl;
                        memset(buffer,0,1025);

                        if (bookingHandler.bookPlace(place)) {
                            sprintf(buffer,"Booked place: (%c%d)\n\0",place.row, place.col);
                            send(sd , buffer , strlen(buffer) +1 , 0 );
                        }
                        else{
                            sprintf(buffer, "Could not book place: (%c%d)\n\0",place.row, place.col);
                            send(sd , buffer , strlen(buffer)+1 , 0 );
                        }
                        bookingHandler.printPlacesDistribution();
                    }
                }
            }
        }
    }


}

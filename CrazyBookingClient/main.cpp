#include<iostream>
#include<sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
//#include <string>
#include <string.h>
#include <list>
#include <thread>

#include "../common_definitions.h"
#include "timer.h"

using namespace std;

char *chooseRandomPlace(char *freePlaceList) {
    int nPlaces;
    sscanf(freePlaceList, "%d,%s", &nPlaces, freePlaceList);
    int place = rand()%nPlaces;

    char *placeName = (char*) malloc(4);
    memset(placeName,0,4);
    int pos = 0;
    while(place!=0) {
        if (freePlaceList[pos]==',')
            place--;
        pos++;
    }

    int i = 0;
    while(freePlaceList[pos]!=',')
        placeName[i++] = freePlaceList[pos++];

    return placeName;
}


int waitRandom() {
    int wait_ms = rand()%41 + 10;
    this_thread::sleep_for(chrono::milliseconds(wait_ms));
    return wait_ms;
}

int main(int argc, char *argv[])
{
    srand((unsigned) time(NULL));

    //Creating a socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock==-1){
        cout << "Cannot create the socket... Aborting.\n";
        return 1;
    }

//    Create a hint structure
    string ipAddress = "127.0.0.1";

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(PORT);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

//    Connect to the server on the socket
    int connectRes = -1;
    for(int i=0; i<10 && connectRes==-1; ++i) {
        connectRes =  connect(sock, (sockaddr*)&hint, sizeof(hint));
        if (connectRes==-1) {
            perror("Could not connect to the server");
            if (i==9) {
                cout << "ABORTING\n";
                exit(EXIT_FAILURE);
            }
            cout << "Trying to reconnect in 5 seconds...\n\n";
            this_thread::sleep_for(chrono::milliseconds(5000));
        }
    }

    char buff[SIZE];
    string userInput;

//  GETTING SERVER GREETINGS
    memset(buff, 0, SIZE);
    int bytesReceived = recv(sock, buff, SIZE, 0);
    cout << "SERVER> " << string(buff,bytesReceived) << "\r\n";

//  WHILE LOOP
    do {
        cout << "> Do you want to book a place? (y/n) ";
        int answ = getchar();
        cin.ignore();
        if (answ=='n') break;
        if (answ!='y') {putchar('\n'); continue;}

        Timer t;


        // GETTING THE LIST OF FREE PLACES
        cout << "> Getting the list of free places\r\n";
        int sendRes = send(sock, FREE_LIST_PLACES_REQUEST, sizeof(FREE_LIST_PLACES_REQUEST) +1, 0);
        if (sendRes == -1) {
            cout << "Could not send the request for the list of free places!\r\n";
            continue;
        }
        memset(buff, 0, SIZE);
        bytesReceived = recv(sock, buff, SIZE, 0);

//        Display response
        cout << "SERVER> " << buff << "\r\n";
        if (buff[0]<'1' || buff[0]>'9') {
            if (buff[0]=='0') {
                cout << "There are no free places left!\r\n";
                return 0;
            }
            else {
                cout << "Something went wrong...";
                break;
            }
        }

        waitRandom();


        char* place = chooseRandomPlace(buff);

        cout << "> ... choosing random place: " << place <<endl;
        userInput = BOOK_REQUEST + string(place);
        cout << "==============> " << userInput <<endl;
//        send to server
        sendRes = send(sock, userInput.c_str(), userInput.size() +1, 0);
        if (sendRes == -1) {
            cout << "Could not send to server!\r\n";
            continue;
        }

//        wait for respond
        memset(buff, 0, SIZE);
        bytesReceived = recv(sock, buff, SIZE, 0);

//        Display response
        cout << "SERVER> " << buff<< "\r\n";
        free(place);
    } while (true);

//    close socket
    close(sock);


    return 0;
}

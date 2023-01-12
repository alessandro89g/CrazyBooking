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
#include <vector>
#include <mutex>
#include <chrono>

#include "../common_definitions.h"
#include "timer.h"
#include "network.h"

using namespace std;
using namespace chrono;
static unsigned int idCount = 0;
Network net(PORT);

typedef struct Request {
    int msToWait = -1;
    int msWaited = 0;
    unsigned int id = -1;
    char freePlaces[1024];
    int freePlacesNumber;
} Request;


static Request requests[250];
static int reqSize = 0;
static bool fullyBooked = false;
mutex mutex_req;



void requestMaker() {
    char buff[SIZE];
    int bytesReceived;
    while (!fullyBooked) {
        this_thread::sleep_for(milliseconds(2));
        if (reqSize<250) {
            for (int i=0; i<250; ++i) {
                std::lock_guard<std::mutex> guard(mutex_req);
                if (requests[i].msToWait <0){
                    if (!net.sendRequest(FREE_LIST_PLACES_REQUEST, sizeof(FREE_LIST_PLACES_REQUEST),buff, bytesReceived)) {
                        cout << "REQUEST > Could not send/receive request\n";
                        continue;
                    }
                    sscanf(buff, "%d,%s", &(requests[i].freePlacesNumber), requests[i].freePlaces);

            //        Display response
                    if (buff[0]<'1' || buff[0]>'9') {
                        if (buff[0]=='0' && requests[i].freePlacesNumber==0) {
                            cout << "REQUEST > THERE ARE NO FREE PLACES LEFT!\r\n";
                            fullyBooked = true;
                            break;
                        }
                        else {
                            cout << "REQUEST > Something went wrong..." << endl;
                            cout << "REQUEST > ERROR > " << buff <<endl;
                            break;
                        }
                    }
                    requests[i].msWaited = 0;
                    requests[i].msToWait = rand()%41+10;
                    requests[i].id = ++idCount;
                    reqSize++;
                    cout << "REQUEST > Added request id " << idCount << " with waiting time " << requests[i].msToWait << " ms" << endl;
                }
            }
        }
    }
}



char *chooseRandomPlace(char *freePlaceList, int numberFreePlaces) {
    int place = rand()%numberFreePlaces;

    char *placeName = (char*) malloc(4);
    memset(placeName,0,4);
    int pos = 0;
    while(place!=0) {
        if (freePlaceList[pos]==',')
            place--;
        pos++;
    }

    int i = 0;
    while(freePlaceList[pos]!=',' && freePlaceList[pos]!=0)
        placeName[i++] = freePlaceList[pos++];

    return placeName;
}


bool elaborateRequest(Request & r, char *&place) {
    place = chooseRandomPlace(r.freePlaces, r.freePlacesNumber);

    char req[20], res[SIZE];
    int resSize;
    sprintf(req, "%s%s",BOOK_REQUEST,place);

    if (net.sendRequest(req,sizeof(req),res, resSize)){
        char booked[] = "Booked", check []= "      ";
        memcpy(check,res,6);
        return strcmp(check,booked)==0;
    }
    else
        return false;
}


void requestProcesser() {
    while (!fullyBooked) {
        this_thread::sleep_for(microseconds(1000));
        for (int i=0; i<250; ++i) {
            std::lock_guard<std::mutex> guard(mutex_req);
            if (requests[i].msToWait > -1){
                requests[i].msToWait--;
                requests[i].msWaited++;
                if (requests[i].msToWait==0){
                    // Book a place from the available ones
                    char *place;
                    if (elaborateRequest(requests[i], place))
                        cout << "BOOKING: " << "Request with ID  "<<requests[i].id
                             << " booked succefully place " << place << " after " << requests[i].msWaited  << " ms" << endl;
                    else
                        cout << "BOOKING: " << "Request with ID  "<<requests[i].id
                             << " booked unsuccessfully :(" << endl;
                    if (place)
                        free(place);
                    reqSize--;
                    requests[i].msToWait--;
                }
            }
        }
    }
}



int main(int argc, char *argv[])
{
    srand((unsigned) time(NULL));

    net.connectToServer();

    thread requestMakerThread(requestMaker);
    thread requestProcesserThread(requestProcesser);
    requestMakerThread.join();
    requestProcesserThread.join();

    net.closeConnection();


    return 0;
}

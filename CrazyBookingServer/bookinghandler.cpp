#include "bookinghandler.h"
#include <fstream>
#include <string.h>

using namespace std;


BookingHandler::BookingHandler(const char *fileName) :
    file(fileName)
{

    if (file.fileExists()) {
        //read file
        freePlacesNumber = file.read(table,DIM);
    }
    else {
        //initialize file
        cout << "New file \n";
        memset(table, false, DIM);

        if (file.write(table, DIM))
            cout << "File created\n";
        freePlacesNumber = DIM;
    }
}

BookingHandler::~BookingHandler()
{
}

void BookingHandler::printPlacesDistribution() const
{
    cout << "====================================" << endl;
    cout << "  ";
    for (int j = 0; j<NCOL; j++)
        printf("%2d ", j+1);
    putchar('\n');
    for (int i=0; i<NROW; i++){
        printf("%c  ", 'A'+i);
        for (int j=0; j<NCOL; j++)
            cout << (table[indx(i,j)]==0 ? 'o' : 'x') << "  ";
        putchar('\n');
    }
    cout << "====================================" << endl;
}

vector<Place> BookingHandler::getFreePlaces() const
{
    vector<Place> l_free;
    l_free.reserve(freePlacesNumber);
    for (int i = 0; i < NROW; ++i) {
        for (int j = 0; j < NCOL; ++j) {
            if (!table[indx(i,j)])
                l_free.push_back({(char) ('A'+i),(short) (j+1)});
        }
    }
    return l_free;
}

const char *BookingHandler::getFreePlacesList() const
{
    char *freePlaces = new char[1024];
    memset(freePlaces,0,1024);

    sprintf(freePlaces, "%d", freePlacesNumber);
    for(Place& p : getFreePlaces())
        sprintf(freePlaces,"%s,%c%d", freePlaces, p.row, p.col);

    freePlaces[strlen(freePlaces)] = '\n';

    return freePlaces;
}

bool BookingHandler::bookPlace(const Place &place)
{
    int row = place.row -'A';
    int col = place.col-1;

    if (row<0 || row >= NROW || col <0 || col >= NCOL) {
        cerr << "bookPlace: PROBLEM WITH BOOKING, WRONG ROW OR COL";
        return false;
    }

    int index = indx(row,col);
    if (table[indx(row,col)]) {
        cerr << "bookPlace: Place already booked!" << endl;
        return false;
    }
    table[indx(row,col)] = true;
    if (file.write(table,DIM)) {
        freePlacesNumber--;
        return true;
    }
    else {
        "bookPlace: COULD NOT WRITE TO FILE";
    }
    return false;
}

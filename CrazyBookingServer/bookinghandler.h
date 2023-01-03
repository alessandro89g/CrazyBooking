#ifndef BOOKINGHANDLER_H
#define BOOKINGHANDLER_H

#include <iostream>
#include <string.h>
#include <fstream>
#include "filehandler.h"
#include <vector>
#include <string>

using namespace std;
#define NROW 10
#define NCOL 20
#define DIM NROW*NCOL

typedef struct Place
{
    char row;
    short col;
    void print() {
        cout << "("  << row << col << ')';
    }
} Place;


inline int indx(int i, int j) {
    return i*NCOL+j;
}

inline int indx(const Place& place) {
    return indx(place.row-'A',place.col);
}




class BookingHandler
{
public:
    BookingHandler(const char *fileName);
    ~BookingHandler();

    void printPlacesDistribution() const;
    vector<Place> getFreePlaces () const;
    const char* getFreePlacesList () const;
    bool bookPlace(const Place &place);
    int getNumberFreePlaces() const {return freePlacesNumber;}
    bool isPlaceOccupied(const Place& place) const {
        return table[indx(place)];
    }


private:
    FileHandler file;
    bool table[DIM];
    int freePlacesNumber = 0;

};

#endif // BOOKINGHANDLER_H

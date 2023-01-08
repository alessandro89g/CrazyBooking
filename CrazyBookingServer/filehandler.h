#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <string>
#include <iostream>
#include <fstream>
#include <syslog.h>

using namespace std;

class FileHandler
{
public:
    FileHandler(const char* fileName);
    ~FileHandler();

    bool fileExists();
    bool write(bool *data, int dim);

    int read(bool *data, int dim);
private:
    const char* fileName;
    FILE *file;
};

#endif // FILEHANDLER_H

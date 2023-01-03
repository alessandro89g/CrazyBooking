#include "filehandler.h"

FileHandler::FileHandler(const char *fileName)
    : fileName(fileName)
{
}

FileHandler::~FileHandler()
{
}

bool FileHandler::fileExists()
{
    file = fopen(fileName,"r");
    if (file == NULL) return false;
    else {
        fclose(file);
        return true;
    }
}

bool FileHandler::write(bool *data, int dim)
{
    file = fopen(fileName,"w");
    if (file == NULL)
        return false;
    for(int i = 0; i<dim; i++)
        if(fprintf(file,"%d", data[i])<0)
            return false;
    fclose(file);
    return true;
}

int FileHandler::read(bool *data, int dim)
{
    file = fopen(fileName,"r");
    if (file == NULL) return -1;

    int count_free = 0;
    for(int i = 0; i<dim; i++){
        char tmp;
        fscanf(file,"%c", &tmp);
        data[i] = tmp-'0';
        if (!data[i])
            ++count_free;
    }
    fclose(file);
    return count_free;
}

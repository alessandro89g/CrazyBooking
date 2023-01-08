#include "filehandler.h"

FileHandler::FileHandler(const char *fileName)
    : fileName(fileName)
{
    syslog(LOG_NOTICE, "Initializing FileHandler...");
}

FileHandler::~FileHandler()
{
}

bool FileHandler::fileExists()
{
    syslog(LOG_NOTICE, "File exists?");
    file = fopen(fileName,"r");
    syslog(LOG_NOTICE, "File opened? %ld", (long) file);
    if (file == NULL) {
        syslog(LOG_NOTICE, "File does not exist/cannot be opened");
        return false;
    }
    else {
        fclose(file);
        return true;
    }
}

bool FileHandler::write(bool *data, int dim)
{
    file = fopen(fileName,"w");
    if (file == NULL){
        syslog(LOG_ERR, "File \"%s\" cannot be opened", fileName);
        syslog(LOG_ERR, "daemon_crazybookingserver stopped for not being able to open file!");
        exit(EXIT_FAILURE);
        return false;
    }
    for(int i = 0; i<dim; i++)
        if(fprintf(file,"%d", data[i])<0){
            syslog(LOG_ERR, "File \"%s\" writing error", fileName);
            return false;
        }
    fclose(file);
    return true;
}

int FileHandler::read(bool *data, int dim)
{
    file = fopen(fileName,"r");
    if (file == NULL) {
        syslog(LOG_NOTICE, "daemon_crazybookingserver file \"%s\" cannot be opened!", fileName);
        syslog(LOG_ERR, "daemon_crazybookingserver stopped for not being able to open file!");
        exit(EXIT_FAILURE);

    }

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

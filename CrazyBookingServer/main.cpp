#include <filesystem>
#include "crazybookingserver.h"


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <string.h>


// Exits on SIGUSR1
void s_handler (int signum)
{
    syslog (LOG_NOTICE, "daemon_crazybookingserver stopped by SIGUSR1.");
    closelog();
    exit(EXIT_SUCCESS);
}

//Daemonization function
static void daemonize()
{
    pid_t pid;

    pid = fork();
    if (pid < 0)
        exit(EXIT_FAILURE);
    if (pid > 0)
        exit(EXIT_SUCCESS);

    if (setsid() < 0)
        exit(EXIT_FAILURE);


    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);
    signal(SIGINT, SIG_IGN);
    signal(SIGUSR1, s_handler);

    pid = fork();
    if (pid < 0)
        exit(EXIT_FAILURE);
    if (pid > 0)
        exit(EXIT_SUCCESS);

    umask(0);

    chdir("/");

    for (int x = sysconf(_SC_OPEN_MAX); x>=0; x--)
        close (x);

    openlog ("daemon_crazybookingserver", LOG_PID, LOG_DAEMON);
}


int main(int argn, char** argv)
{
    string filename;
    if (argn==1){
        std::filesystem::path cwd = std::filesystem::current_path() / "booking.txt";
        filename = cwd.string();
    }
    else if (strchr(argv[1], '/') != NULL)
        filename =argv[1];
    else {
        std::filesystem::path cwd = std::filesystem::current_path() / argv[1];
        filename = cwd.string();
    }
    const char * fname = filename.c_str();

    daemonize();

    syslog (LOG_NOTICE, "daemon_crazybookingserver started.");

    CrazyBookingServer *server;
    syslog (LOG_NOTICE, "daemon_crazybookingserver file set to: %s", fname);
    server = new CrazyBookingServer(fname);

    server->run();

    syslog (LOG_NOTICE, "daemon_crazybookingserver stopped.");
    closelog();

    return EXIT_SUCCESS;
}


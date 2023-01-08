//Example code: A simple server side code, which echos back the received message.
//Handle multiple socket connections with select and fd_set on Linux

#include <filesystem>

#include "crazybookingserver.h"

using namespace std;

int main(int argn, char** argv)
{
    CrazyBookingServer *server;

    if (argn==1){
        std::filesystem::path cwd = std::filesystem::current_path() / "booking.txt";
        cout << "CrazyBooking file set to: " << cwd.string().c_str() << endl;
        server = new CrazyBookingServer(cwd.string().c_str());
    }
    else{
        cout << "CrazyBooking file set to: " << argv[1] << endl;
        server = new CrazyBookingServer(argv[1]);
    }


    server->run();

    return 0;
}

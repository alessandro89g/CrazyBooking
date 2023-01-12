# CrazyBooking

CrazyBookingServer
The server can be run with or without and argument: the file where the free places will be saved.
Example: 	crazybooking-server /home/booking.dat

It runs as a daemon and exits on SIGUSR1.


CrazyBookingClient
The first type of client allows the user to book a place at a time.

CrazyBookingClient2
The second type of the client runs up to 250 parrallel queries at a time, until the server gives the infomation that there are no places available left. Then the program exits.

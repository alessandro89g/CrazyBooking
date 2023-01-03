.PHONY = all clean

CC = g++-10
CFLAGS = -pthread -g -Wall -O3  -std=gnu++1z

all: main.cpp bookinghandler.cpp filehandler.cpp bookinghandler.h filehandler.h
	$(CC) $(CFLAGS) $^ -o crazybooking-server
	
clean:
	$(RM) crazybooking-server 

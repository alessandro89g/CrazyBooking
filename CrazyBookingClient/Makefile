.PHONY = all clean

CC = g++-10
CFLAGS = -g -Wall -O3  -std=gnu++1z

all: main.cpp timer.cpp timer.h 
	$(CC) $(CFLAGS) $^ -o crazybooking-client	
clean:
	$(RM) crazybooking-client

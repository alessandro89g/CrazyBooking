.PHONY = all clean

CC = g++-10
CFLAGS = -g -Wall  -std=gnu++1z -ggdb3
LIBS = -pthread

all: main.cpp timer.cpp timer.h network.h network.cpp 
	$(CC) $(CFLAGS) $(LIBS)  $^ -o crazybooking-client	
clean:
	$(RM) crazybooking-client

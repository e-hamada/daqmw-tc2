CC = g++
PROG = read_file_decode
CXXFLAGS = -g -O2 -Wall
# LDLIBS += -L/usr/local/lib/ -lmylib

OBJS += $(PROG).o
OBJS += RawDataPacket.o

all: $(PROG)

$(PROG): $(OBJS)
RawDataPacket.o: RawDataPacket.h RawDataPacket.cpp

clean:
	rm -f *.o $(PROG)

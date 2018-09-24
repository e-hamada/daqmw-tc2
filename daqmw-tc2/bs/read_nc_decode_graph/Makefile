CC = g++
PROG = read_nc_decode_graph
CXXFLAGS = -g -O2 -Wall
CPPFLAGS += -I$(shell ${ROOTSYS}/bin/root-config --incdir)
LDLIBS   += $(shell ${ROOTSYS}/bin/root-config --glibs)

# LDLIBS += -L/usr/local/lib/ -lmylib

OBJS += $(PROG).o
OBJS += RawDataPacket.o

all: $(PROG)

$(PROG): $(OBJS)
RawDataPacket.o: RawDataPacket.h RawDataPacket.cpp

clean:
	rm -f *.o $(PROG)

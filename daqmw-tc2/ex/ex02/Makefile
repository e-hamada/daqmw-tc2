CC = g++
PROG = main
CXXFLAGS = -g -O2 -Wall
# LDLIBS += -L/usr/local/lib/ -lmylib

OBJS += main.o
OBJS += MyClass.o

all: $(PROG)
$(PROG): $(OBJS)

clean:
	rm -f *.o $(PROG)

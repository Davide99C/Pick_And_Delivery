CC=gcc
CCOPTS=-Wall -std=gnu99 -O3
CXX=g++
CXXOPTS=-Wall -std=gnu++11 -O3

#COBJS=proc_control.o
HEADERS= websocket.h 
#OBJS=proc_shell.o env.o

.phony=clean all

BINS= web reader_db#proc_shell simple
all:	$(BINS)


%.o:	%.c $(HEADERS)
	$(CC) -c $(CCOPTS) $<

%.o:	%.cpp $(HEADERS)
	$(CXX) -c $(CXXOPTS) $<

web:	main.cpp websocket.o 
	$(CXX) -o $@ $^ $(CXXOPTS) -lpthread -lwebsockets

reader_db: reader_db.cpp 
	$(CXX) -o $@ $^ $(CXXOPTS) -lmysqlclient -lssl -lcrypto -lpthread -L/usr/include/mysql
#proc_shell:	proc_shell_main.cpp $(COBJS) $(OBJS)
#	$(CXX) -o $@  $^ $(CXXOPTS) -lpthread 

#simple:  simple.c
#	$(CC) -o $@ $^

clean:
	rm -f *~ *.o $(BINS)

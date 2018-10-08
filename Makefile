OBJS     = third.o LRU.o ws.o Inv_ptable.cpp
SOURCE   = third.cpp LRU.cpp ws.cpp Inv_ptable.cpp
HEADER   = Inv_ptable.h LRU.h ws.h
OUT      = test
CC       = g++
FLAGS 	 = -g3 -c

$(OUT): $(OBJS)
	$(CC) -o $@ $(OBJS) -g3

simulator.o: simulator.cpp
	$(CC) $(FLAGS) simulator.cpp

LRU.o: LRU.cpp
	$(CC) $(FLAGS) LRU.cpp

ws.o: ws.cpp
	$(CC) $(FLAGS) ws.cpp
clean:
	rm -f test
run:
	./test 0 1024 32 5 50000
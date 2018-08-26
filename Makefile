CXXFLAGS = -Wall -g -Ofast

pi: pi.o
	g++ -g -o pi pi.o -lpthread

clean:
	rm -f *.o *~ *.stackdump

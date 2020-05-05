COMPILE_OPTIONS = -Wall -Wextra -std=c++11 -O3
GRAPHDIR = ../../../lib/Graph/
BHEAPDIR = ../../../lib/BinHeap/

main.exe: main.o
	g++ -o main.exe main.o 

#dipende da WL_Greedy.h
main.o: main.cpp $(GRAPHDIR)graph.h $(BHEAPDIR)binheap.h
	g++ -c $(COMPILE_OPTIONS) main.cpp $(GRAPHDIR)graph.h $(BHEAPDIR)binheap.h

clean:
	rm -f *.o *.exe *.gch

LIB_PATH = ../../../lib
BRKGA_PATH = $(LIB_PATH)/BRKGA
GRAPH_PATH = $(LIB_PATH)/Graph
BINHEAP_PATH = $(LIB_PATH)/BinHeap
CELL_PATH = $(LIB_PATH)/Cell
CIRCUIT_PATH = $(LIB_PATH)/Circuit
CGI_PATH = $(LIB_PATH)/CircuitGraphInterface

FLAGS = -std=c++17 -Wall -O3 # -g

OBJECT_FILES = cell.o circuit.o circuitgraphinterface.o main.o

main.exe: $(OBJECT_FILES)
	g++ $(OBJECT_FILES) -o main.exe

main.o: main.cpp $(CGI_PATH)/circuitgraphinterface.h  $(CIRCUIT_PATH)/circuit.h $(CELL_PATH)/cell.h
	g++ -c $(FLAGS) main.cpp $(CGI_PATH)/circuitgraphinterface.h

circuitgraphinterface.o: $(CGI_PATH)/circuitgraphinterface.h $(CGI_PATH)/circuitgraphinterface.cpp $(CIRCUIT_PATH)/circuit.h $(CELL_PATH)/cell.h
	g++ -c $(FLAGS) $(CGI_PATH)/circuitgraphinterface.cpp 

circuit.o: $(CIRCUIT_PATH)/circuit.h $(CIRCUIT_PATH)/circuit.cpp $(CELL_PATH)/cell.h 
	g++ -c $(FLAGS) $(CIRCUIT_PATH)/circuit.cpp

cell.o: $(CELL_PATH)/cell.h $(CELL_PATH)/cell.cpp 
	g++ -c $(FLAGS) $(CELL_PATH)/cell.cpp

clean:
	rm -f *.o *.exe $(CELL_PATH)/*.gch $(CIRCUIT_PATH)/*.gch $(CGI_PATH)/gch

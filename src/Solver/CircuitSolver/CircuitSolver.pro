TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++20

#QMAKE_CXXFLAGS_DEBUG += -O3
#QMAKE_CXXFLAGS_RELEASE += -O3
#QMAKE_CXXFLAGS += -O3

SOURCES += \
        ../../../lib/Cell/cell.cpp \
        ../../../lib/Circuit/circuit.cpp \
        ../../../lib/CircuitGraphInterface/circuitgraphinterface.cpp \
        ../../../lib/CircuitSolver/circuitsolver.cpp \
        main.cpp

HEADERS += \
    ../../../lib/BRKGA/BRKGA.h \
    ../../../lib/BinHeap/binheap.h \
    ../../../lib/Cell/cell.h \
    ../../../lib/Circuit/circuit.h \
    ../../../lib/CircuitGraphInterface/circuitgraphinterface.h \
    ../../../lib/CircuitSolver/circuitsolver.h \
    ../../../lib/Graph/graph.h \
    ../../../lib/SingletonMutex/mutex.h

SYSTEM     = x86-64_linux
LIBFORMAT  = static_pic
CPLEXDIR = /shared/home/ihuerta/ibm/ILOG/CPLEX_Studio1271/cplex
CONCERTDIR = /shared/home/ihuerta/ibm/ILOG/CPLEX_Studio1271/concert
CCC = g++
CCOPT = -DNDEBUG -DIL_STD -std=c++11 -fpermissive
CPLEXBINDIR   = $(CPLEXDIR)/bin/$(BINDIST)
CPLEXLIBDIR   = $(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CONCERTLIBDIR = $(CONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CCLNFLAGS = -L$(CPLEXLIBDIR) -lilocplex -lcplex -L$(CONCERTLIBDIR) -lconcert -lm -pthread
CLNFLAGS  = -L$(CPLEXLIBDIR) -lcplex -lm -pthread
CONCERTINCDIR = $(CONCERTDIR)/include/
CPLEXINCDIR   = $(CPLEXDIR)/include/
CCFLAGS = $(CCOPT) -I$(CPLEXINCDIR) -I$(CONCERTINCDIR)

all: main

main: main.o

	$(CCC) $(CCFLAGS) main.o -o cplex $(CCLNFLAGS)

main.o: main.cpp

	$(CCC) -c $(CCFLAGS) -O3 main.cpp -o main.o 

clean:
	@rm -f *~ *.o main core


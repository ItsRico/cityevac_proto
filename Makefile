evac.out : evac.o EvacRunner.o graph.o
	g++ -ansi -Wall -g -o evac.out evac.o EvacRunner.o graph.o

evac.o : evac.cpp evac.h BinaryHeap.cpp
	g++ -ansi -Wall -g -c evac.cpp

EvacRunner.o : EvacRunner.cpp CPUTimer.h EvacRunner.h evac.h
	g++ -ansi -Wall -g -c EvacRunner.cpp

graph.o: graph.cpp graph.h
		g++ -ansi -Wall -g -c graph.cpp
clean :
	rm -f evac.out evac.o  EvacRunner.o graph.o

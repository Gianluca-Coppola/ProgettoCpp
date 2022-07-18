CXX = g++
#CXXFLAGS = -DC_DEBUG


main.exe: main.o
	$(CXX) $(CXXFLAGS) main.o -o main.exe

main.o: main.cpp graph.h Persona.h
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

.PHONY: clean
clean:
	rm *.o *.exe

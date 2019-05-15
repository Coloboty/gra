SFML= -lsfml-graphics -lsfml-window -lsfml-system

main: main.o
	g++ main.o -o main ${SFML}

main.o: main.cpp
	g++ -c main.cpp

run: main
	./main

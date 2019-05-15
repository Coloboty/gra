SFML= -lsfml-graphics -lsfml-window -lsfml-system

EXEC= main

CFLAGS= -Iinc -Wall -pedantic

$(EXEC): obj/main.o obj/f_pomocnicze.o
	g++ obj/main.o obj/f_pomocnicze.o -o main ${SFML}

obj/main.o: src/main.cpp inc/f_pomocnicze.h
	g++ -c src/main.cpp -o obj/main.o $(CFLAGS)

obj/f_pomocnicze.o: src/f_pomocnicze.cpp inc/f_pomocnicze.h
	g++ -c src/f_pomocnicze.cpp -o obj/f_pomocnicze.o $(CFLAGS)

run: main
	./main

clean:
	rm obj/*.o $(EXEC)

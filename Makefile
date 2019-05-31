SFML= -lsfml-graphics -lsfml-window -lsfml-system

EXEC= main

CFLAGS= -Iinc -Wall -pedantic

$(EXEC): obj/main.o obj/f_pomocnicze.o obj/plansza.o obj/ai.o
	g++ -o main obj/main.o obj/f_pomocnicze.o obj/plansza.o obj/ai.o ${SFML}

obj/main.o: src/main.cpp inc/f_pomocnicze.h inc/plansza.h inc/ai.h inc/config.h
	g++ -c src/main.cpp -o obj/main.o $(CFLAGS)

obj/f_pomocnicze.o: src/f_pomocnicze.cpp inc/f_pomocnicze.h
	g++ -c src/f_pomocnicze.cpp -o obj/f_pomocnicze.o $(CFLAGS)

obj/plansza.o: src/plansza.cpp inc/plansza.h inc/f_pomocnicze.h inc/config.h inc/union_find.hpp
	g++ -c src/plansza.cpp -o obj/plansza.o $(CFLAGS)

obj/ai.o: src/ai.cpp inc/ai.h inc/f_pomocnicze.h inc/config.h inc/union_find.hpp
	g++ -c src/ai.cpp -o obj/ai.o $(CFLAGS)

run: main
	./main

clean:
	rm obj/*.o $(EXEC) src/*~ src/#*# inc/*~ inc/#*# 2> /dev/null || true

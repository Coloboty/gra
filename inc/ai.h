#ifndef AI_H
#define AI_H

#include "config.h"
#include "plansza.h"
#include <string>
#include <cstdlib>
#include <SFML/Graphics.hpp>

#define WIN 2147483647
#define FAIL -WIN


using namespace std;
using namespace sf;

class ai{
    uint me, opponent, idepth;
public:
    ai(uint _me, uint _d){
	me= _me;
	idepth= _d;
	if(me == 1) opponent= 2;
	else opponent= 1;

	srand(time(NULL));
    }

    int thonk(const game_state &state) const;
    int minMax(game_state &state) {return minMax(state, idepth, FAIL, WIN, me);}
    int minMax(game_state &state, uint depth, int alpha, int beta, uint p);
    vector<game_state> validMoves(const game_state &current, uint player) const;
};



#endif

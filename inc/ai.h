#ifndef AI_H
#define AI_H

#include "config.h"
#include "plansza.h"
#include <string>
#include <cstdlib>
#include <SFML/Graphics.hpp>

#define FAIL -1000
#define WIN 1000

using namespace std;
using namespace sf;

class ai{
    uint me, opponent;
public:
    ai(uint _me){
	me= _me;
	if(me == 1) opponent= 2;
	else opponent= 1;

	srand(time(NULL));
    }

    int thonk(const game_state &state) const;
    int minMax(game_state &state, uint depth, uint p);
    vector<game_state> validMoves(const game_state &current, uint player) const;
};



#endif

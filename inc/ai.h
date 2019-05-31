#ifndef AI_H
#define AI_H

#include "config.h"
#include "plansza.h"
#include <cstring>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <memory>

using namespace std;
using namespace sf;

class move_state: public game_state{
public:
    Vector2u pos;
    
    move_state(game_state &s){
	grid= s.grid;
    }
};

/* ---------------------------------------------- */

class node{
public:
    vector<move_state> children;

    node(vector<move_state> c){
	children= c;
    }

    node() {}
};

/* ---------------------------------------------- */

class ai{
    uint player;
public:
    ai(uint _player){
	player= _player;
    }

    int thonk(move_state state){
	int gains= 0;

	gains= state.count(state.pos);

	return gains;
    }

    void buildTree(game_state &current, uint depth){
	vector<node> nodes;
	vector<move_state> moves;

	/* root */
	nodes.push_back(validMoves(current));
	
	for(uint i= 0; i < depth; i++){
	    
	}
    }
    
    vector<move_state> validMoves(game_state &current){
	vector<move_state> result;
	Vector2u pos;

	for(uint i= 0; i < BOARD_SIZE_X; i++){
	    if(current.isValidMove(i)){
		result.push_back(current);
		pos= result.back().addDot(player, i);
		result.back().pos= pos;
	    }
	}

	return result;
    }
};



#endif

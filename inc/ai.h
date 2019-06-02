#ifndef AI_H
#define AI_H

#include "config.h"
#include "plansza.h"
#include <cstring>
#include <SFML/Graphics.hpp>
#include <stack>
#include <limits>

/* #define INF std::numeric_limits<int>::max() */
#define INF 1000
#define FAIL -1000
#define WIN 1000

using namespace std;
using namespace sf;

class move_state: public game_state{
public:
    Vector2u pos;

    move_state() {}
    move_state(game_state &s){
	grid= s.grid;
    }

    void operator =(game_state &s) {grid= s.grid;}
    void operator =(move_state &s) {grid= s.grid;}

    bool checkWin(void) {return count(pos) >= 4;}

    void printState(void){
	for(uint i= 0; i < BOARD_SIZE_Y; i++){
	    for(uint j= 0; j < BOARD_SIZE_X; j++){
		cout << getGridState(Vector2u(j, i)) << "   ";
	    }
	    cout << endl;
	}
    }

    /* move_state(move_state &s){ */
    /* grid= s.grid; */
    /* } */
};

/* ---------------------------------------------- */

class node{
public:
    vector<node> children;
    move_state state;
    uint depth;
    int value;

    node(move_state s) {state= s;}
    node() {}
};

/* ---------------------------------------------- */

class ai{
    uint me, opponent;
public:
    ai(uint _me){
	me= _me;
	if(me == 1) opponent= 2;
	else opponent= 1;
    }

    int thonk(move_state &state){
	int gain= 0;

	if(state.checkWin())
	    gain= WIN;
	else
	    gain= state.count(state.pos) * 5;

	if(state.getGridState(state.pos) == me){
	    /* cout << "gain " << gain << endl; */
	    return gain;
	}
	else{
	    /* cout << "gain " << -gain << endl; */
	    return -gain;
	}
    }

    int minMax(move_state &state, uint depth, uint p){
	vector<move_state> moves;
	int result, last;
	
	if(depth == 0 || state.checkWin()){
	    /* state.printState(); */
	    return thonk(state) + depth;
	}

	/* Gracz maksymalizujący (ja) */
	if(p == me){
	    moves= validMoves(state, me);
	    result= FAIL;
	    
	    for(uint i= 0; i < moves.size(); i++){
		last= minMax(moves[i], depth - 1, opponent);
		if(last >= result){
		    result= last;
		    state.pos= moves[i].pos;
		}
	    }

	    
	    return result;
	}
	/* Gracz minimalizujący (przeciwnik) */
	else{
	    moves= validMoves(state, opponent);
	    result= WIN;

	    for(uint i= 0; i < moves.size(); i++){
		last= minMax(moves[i], depth - 1, me);
		result= min(result, last);
	    }

	    /* cout << "ewaluuje " << thonk(state, p)<< endl; */
	    return result;
	}
    }

    vector<move_state> validMoves(const move_state &current, uint player){
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

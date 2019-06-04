#include "ai.h"

int ai::thonk(const game_state &state) const{
    int gain= 0;
    uint p;

    for(uint i= 0; i < BOARD_SIZE_X; i++){
	for(uint j= 0; j < BOARD_SIZE_Y; j++){
	    p= state.getGridState(Vector2u(i, j));
	    
	    if(state.checkWin(Vector2u(i, j))){
		if(p == me){
		    return WIN;
		}
		else{
		    return FAIL;
		}
	    }
	    else if(p == me)
		gain+= state.countAll(Vector2u(i, j), p);
	    else
		gain-= state.countAll(Vector2u(i, j), p);
	}
    }
    
    return gain;
}

int ai::minMax(game_state &state, uint depth, int alpha, int beta, uint p){
    vector<game_state> moves;
    int result, last;
    int a, b;
    a= alpha;
    b= beta;
	
    if(depth == 0 || state.checkWin())
	return thonk(state);
    

    /* Gracz maksymalizujący (ja) */
    if(p == me){
	moves= validMoves(state, me);
	result= FAIL;
	    
	for(uint i= 0; i < moves.size(); i++){
	    last= minMax(moves[i], depth-1, a, b, opponent);
	    a= max(a, last);
	    
	    if(last >= result){
		result= last;
		state.pos= moves[i].pos;
	    }

	    if(b < a)
		break;
	}

	return result;
    }
    /* Gracz minimalizujący (przeciwnik) */
    else{
	moves= validMoves(state, opponent);
	result= WIN;
	
	for(uint i= 0; i < moves.size(); i++){
	    last= minMax(moves[i], depth-1, a, b, me);
	    result= min(result, last);
	    b= min(b, last);

	    if(b < a)
		break;
	}

	return result;
    }
}

vector<game_state> ai::validMoves(const game_state &current, uint player) const{
    vector<game_state> result;

    for(uint i= 0; i < BOARD_SIZE_X; i++){
	if(current.isValidMove(i)){
	    result.push_back(current);
	    result.back().addDot(player, i);
	}
    }

    return result;
}

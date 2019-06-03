#include "ai.h"

int ai::thonk(const game_state &state) const{
    int gain= 0;
    uint p= state.getGridState(state.pos);

    if(state.checkWin())
	gain= WIN;
    else
	gain= state.count(state.pos, p) * 10;

    
    if(p == me){
	gain+= state.count(state.pos, opponent)*10;
	return gain;
    }
    else{
	gain+= state.count(state.pos, me)*10;
	return gain * (-1);
    }
}

int ai::minMax(game_state &state, uint depth, uint p){
    vector<game_state> moves;
    int result, last;
	
    if(depth == 0 || state.checkWin()){
	return thonk(state) + rand()%6;
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

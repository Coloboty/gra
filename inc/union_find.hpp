#ifndef UNION_FIND_HPP
#define UNION_FIND_HPP

#include <vector>
/* ------------------------------------------------- */

using namespace std;

template<typename T>
class union_find{
    
    struct node{
    	T val;
	uint root, me;
	uint rang;
	node(T _val, uint _root): val(_val), root(_root), me(_root), rang(0) {}
    };

    uint size;
    vector<node> nodes;
public:
    union_find(){
	size= 0;
    }

    ~union_find(){
    }

    /*  */

    uint makeSet(T elem){	
	nodes.push_back(node(elem,nodes.size()));
	return nodes.size()-1;
    }
    void changeVal(T elem, uint i) {nodes[i].val= elem;}
    T getVal(uint i) {return nodes[i].val;}

    uint find(uint x){
	if(nodes[x].root != x)
	    nodes[x].root= find(nodes[x].root);
	return nodes[x].root;
    }

    void join(uint x, uint y){
	uint ox, oy, temp;
	ox= nodes[x].root;
	oy= nodes[y].root;

	if(ox == oy)
	    return;

	if(nodes[ox].rang < nodes[oy].rang){
	    temp= ox;
	    ox= oy;
	    oy= temp;
	}

	nodes[oy].root= ox;
	if(nodes[ox].rang == nodes[oy].rang)
	    nodes[ox].rang++;
    }
};

#endif

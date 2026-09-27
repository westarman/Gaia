#include <algorithm>
#include <vector>
#include "structs.h"

using namespace std;

// REMEMBER!!! -> The graph has NEGATIVE EDGES, make sure you keep mind of them when accessing the board edges

// Alternating grid of hexagons, 'columns' determine number of main columns and always have 'columns-1' number of smaller columns
vector<vector<int>> board_constructor(int columns, int colsize) {

    int nodes = columns*colsize + (columns-1)*(colsize-1); 
    vector<vector<int>> graph(nodes);
    vector<int> tmp(6,-1);

    for(int i = 0; i < nodes; i++) {
        graph[i] = tmp;

        bool main_col = (i % (2*colsize-1)) < colsize;
        bool first_node = (i % (2*colsize-1)) == (main_col ? 0:colsize);
        bool last_node = ((i+1) % (2*colsize-1)) == (main_col ? colsize:0);
        bool first_col = i < colsize;
        bool last_col = i > nodes-colsize-1;

        
        if(!first_node) graph[i][0] = i-1; // connect up
        if((!first_node || !main_col) && !last_col) graph[i][1] = i+colsize-1; // connect top right
        if((!last_node || !main_col) && !last_col) graph[i][2] = i+colsize; //connect bottom right
        
        if(!last_node) graph[i][3] = i+1; // connect down
        if((!last_node || !main_col) && !first_col) graph[i][4] = i-colsize+1; // connect bottom left
        if((!first_node || !main_col) && !first_col) graph[i][5] = i-colsize; // connect top left
        
    }

    return graph;
}

//GLOBAL GRAPH
const vector<vector<int>> board = board_constructor(cols,colsize);
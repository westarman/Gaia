#include <algorithm>
#include <iostream>
#include <vector>
#include "structs.h"

using namespace std;

// Animal template { .score={}, .name="", .anchor=, .middle=, {0,0,0,0,0,0} };
//                                                    {T, TR, BR, B, BL, TL}
// limitations: shapes can only encompass a 1 hop neigborhood of a hex and the root hex must not be empty 
// (AKA the maximum width of the shape is 3 hexes long and you cant have the middle hex empty)
//ANIMAL CARDS
Animal Gecko { .score={0,5,10,16}, .name="Gecko", .anchor=Direction::BR, .middle = State::Field, {0,0,State::Building2,0,0,State::Field} };
Animal Shrew { .score={0,5,10,17}, .name="Shrew", .anchor=-1, .middle=State::Building2, {0,0,State::Field,0,State::Field,0} };
Animal Flamingo { .score={0,4,10,16}, .name="Flamingo", .anchor=-1, .middle=State::River, {0,0,0,0,State::Field,State::Field} };
Animal Meerkat { .score={0,2,5,9,14}, .name="Meerkat", .anchor=-1, .middle=State::Mountain1, {0,0,0,0,0,State::Field} };
Animal Raccoon  { .score={0,6,12}, .name="Raccoon", .anchor=-1, .middle=State::Field, {0,0,State::River,State::River,State::River,0} };
Animal Warthog { .score={0,4,8,13}, .name="Warthog", .anchor=-1, .middle= State::Tree2, {0,0,0,0,0,State::Building2} };

//SPIRITS


//ANIMAL PLACEMENT
// updates possible 'card' anchor positions based on the recent token placement 
// (!Only for middle anchor cards, the non-middle ones do a whole board recompute)
void update_anchor_positions(Animal &card, vector<Tile> &status, int node) {

    // were gonna check the placed tile and all its neighbors as the center of the shape
    vector<int> tiles; 
    tiles.push_back(node);
    for(int x : board[node]) {
        if(x<0) continue;
        tiles.push_back(x);
    }
    
    // reset previous set anchors
    if(card.anchor == -1) {
        for(int x : tiles) card.placement[x] = 0; 
    } else {
        // technically non-middle anchor cards need a 2-hop neigbhorhood recompute,
        // but on the classic game board of 23 tiles thats the majority of the board for any tile
        
        tiles.clear();
        for(int i = 0; i < nodes; i++) tiles.push_back(i); 
        fill(card.placement.begin(),card.placement.end(),0);
    }

    for(int i = 0; i < tiles.size(); i++) {
        int tile = tiles[i]; // <- this is the middle node of our shape on the board
        if(status[tile].state_id() == card.middle) { //check middle

            if(card.anchor == -1) { //one middle anchor

                //check anchor
                if(status[tile].animal) continue;

                //check neigbors & rotations
                vector<int> states(6,0);
                for(int j = 0; j < 6; j++) {
                    int neighbor = board[tile][j];
                    if(neighbor<0) continue;  
                    states[j] = status[neighbor].state_id();
                }
                for(int j = 0; j < 6; j++) {
                    bool match = true;
                    for(int k = 0; k < 6; k++) {
                        if(card.neighbors[k]==0) continue;
                        if(card.neighbors[k]!=states[k]) { match=false; break; }
                    }
                    if(match) { card.placement[tile]=1;  break; }
                    std::rotate(states.begin(), states.begin()+1, states.end());
                }
            
            } else { //multiple non-middle anchors

                //check neigbors & rotations
                vector<int> states(6,0);
                for(int j = 0; j < 6; j++) {
                    int neighbor = board[tile][j];
                    if(neighbor<0) continue;  
                    states[j] = status[neighbor].state_id();
                }
                vector<int> anchors;
                for(int j = 0; j < 6; j++) {
                    bool match = true;
                    for(int k = 0; k < 6; k++) {
                        if(card.neighbors[k]==0) continue;
                        if(card.neighbors[k]!=states[k]) { match=false; break; }
                    }
                    //rotate back to locate anchor on board
                    int dir = (card.anchor+j) % 6; 
                    //check anchor
                    int anchor_tile = board[tile][dir];
                    if(anchor_tile<0 || status[anchor_tile].animal) match = false;
                    if(match) anchors.push_back(anchor_tile);
                    std::rotate(states.begin(), states.begin()+1, states.end());
                }
                for(int x : anchors) card.placement[x]=1;
            }

        }
    }
}

// 13 14 18 {19 20} {26 27} {28 29}

// 0:
// 13 27 28 18 26